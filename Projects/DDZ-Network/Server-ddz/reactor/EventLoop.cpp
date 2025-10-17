#include "EventLoop.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>
#include "SelectDispatcher.h"
#include "PollDispatcher.h"
#include "EpollDispatcher.h"

EventLoop::EventLoop() : EventLoop(std::string())
{
}

EventLoop::EventLoop(const std::string& threadName)
{
	m_dispatcher = new EpollDispatcher(this);
	// 初始化ChannelMap
	m_channelMap.clear();
	m_threadID = std::this_thread::get_id();
	m_threadName = (threadName == std::string()) ? "MainThread" : threadName;
	int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, m_socketPair);
	if (ret == -1)
	{
		perror("socketpair");
		exit(0);
	}
	// 指定规则: evLoop->socketPair[0] 发送数据, evLoop->socketPair[1] 接收数据
#if 0
	Channel* channel = new Channel(m_socketPair[1], FDEvent::ReadEvent, 
		readLocalMessage, nullptr, nullptr, this);
#else
	// 绑定 -- std::bind
	auto obj = std::bind(&EventLoop::readMessage, this);
	Channel* channel = new Channel(m_socketPair[1], FDEvent::ReadEvent,
		obj, nullptr, nullptr, this);
#endif
	// 把 channel 添加到任务队列
	addTask(channel, ElemType::ADD);
}

EventLoop::~EventLoop()
{
}

int EventLoop::run()
{
	m_isQuit = false;
	// 判断线程ID是否正常
	if (m_threadID != std::this_thread::get_id())
	{
		return -1;
	}
	// 循环进行事件处理
	while (!m_isQuit)
	{
		m_dispatcher->dispatch();		// 默认超时时长 2s
		processTaskQueue();
	}
	return 0;
}

int EventLoop::eventActivate(int fd, int events)
{
	if (fd < 0)
	{
		return -1;
	}

	// 取出 channel
	Channel* channel = m_channelMap.at(fd);
	assert(channel->getSocketFd() == fd);
	if ((events & static_cast<int>(FDEvent::ReadEvent)) && channel->readCallback)
	{
		channel->readCallback(const_cast<void*>(channel->getArg()));
	}
	if ((events & static_cast<int>(FDEvent::WriteEvent)) && channel->writeCallback)
	{
		channel->writeCallback(const_cast<void*>(channel->getArg()));
	}

	return 0;
}

int EventLoop::addTask(Channel* channel, ElemType type)
{
	// 加锁, 保护共享资源, 互斥访问任务队列
	m_mutex.lock();
	// 创建新节点
	ChannelElement* node = new ChannelElement;
	node->type = type;
	node->channel = channel;
	m_taskQueue.push(node);
	m_mutex.unlock();

	// 处理节点
	/*
	* 细节:
	*	1. 对于链表节点的添加: 可能是当前线程, 也有可能是其他线程(主线程)
	*		1). 修改fd的事件, 当前子线程发起并处理;
	*		2). 添加新的fd, 添加任务节点的操作由主线程发起;
	*	2. 不能让主线程处理任务队列，需要由当前的子线程去处理
	*/
	if (m_threadID == std::this_thread::get_id())
	{
		// 当前子线程(基于子线程的角度分析)
		processTaskQueue();
	}
	else
	{
		// 主线程 -- 通知子线程处理任务队列中的任务
		// 1. 子线程在工作; 2. 子线程被阻塞: select, poll, epoll
		taskWakeup();
	}
	return 0;
}

int EventLoop::processTaskQueue()
{
	// 从任务队列中取出所有节点
	while (!m_taskQueue.empty())
	{
		m_mutex.lock();
		ChannelElement* node = m_taskQueue.front();
		m_taskQueue.pop();		// 删除节点
		m_mutex.unlock();
		Channel* channel = node->channel;
		if (node->type == ElemType::ADD)
		{
			// 添加
			add(channel);
		}
		else if (node->type == ElemType::DELETE)
		{
			// 删除
			remove(channel);
		}
		else if (node->type == ElemType::MODIFY)
		{
			// 修改
			modify(channel);
		}
		delete node;
	}
	return 0;
}

int EventLoop::freeChannel(Channel* channel)
{
	auto it = m_channelMap.find(channel->getSocketFd());
	if (it != m_channelMap.end())
	{
		// 删除 channel 和 fd 的对应关系
		m_channelMap.erase(it);
		// 关闭文件描述符 fd
		close(channel->getSocketFd());
		// 释放channel
		delete channel;
	}
	return 0;
}

// 读数据, 子线程检测的evLoop->socketPair[1]触发读事件, 线程解除阻塞(类静态成员函数)
int EventLoop::readLocalMessage(void* arg)
{
	EventLoop* evLoop = static_cast<EventLoop*>(arg);
	char buf[256];
	read(evLoop->m_socketPair[1], buf, sizeof(buf));
	return 0;
}

// 读数据, 子线程检测的evLoop->socketPair[1]触发读事件, 线程解除阻塞(类非静态成员函数)
int EventLoop::readMessage()
{
	char buf[256];
	read(m_socketPair[1], buf, sizeof(buf));
	return 0;
}

// 写数据, 主线程通过向evLoop->socketPair[0]写数据唤醒子线程
void EventLoop::taskWakeup()
{
	const char* msg = "Hello World!";
	write(m_socketPair[0], msg, strlen(msg));
}

int EventLoop::add(Channel* channel)
{
	int fd = channel->getSocketFd();
	// 找到fd对应的数组元素位置, 并存储键值对 fd -> channel
	if (m_channelMap.find(fd) == m_channelMap.end())
	{
		m_channelMap.insert(std::make_pair(fd, channel));
		int ret = m_dispatcher->add(channel);
		return ret;
	}
	return -1;
}

int EventLoop::remove(Channel* channel)
{
	int fd = channel->getSocketFd();
	if (m_channelMap.find(fd) == m_channelMap.end())
	{
		return -1;
	}
	int ret = m_dispatcher->remove(channel);
	return ret;
}

int EventLoop::modify(Channel* channel)
{
	int fd = channel->getSocketFd();
	if (m_channelMap.find(fd) == m_channelMap.end())
	{
		return -1;
	}
	int ret = m_dispatcher->modify(channel);
	return ret;
}
