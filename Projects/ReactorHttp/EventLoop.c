#include "EventLoop.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

struct EventLoop* eventLoopInit()
{
	return eventLoopInitEx(NULL);
}

// 写数据, 主线程通过向evLoop->socketPair[0]写数据唤醒子线程
void taskWakeup(struct EventLoop* evLoop)
{
	const char* msg = "Hello World!";
	write(evLoop->socketPair[0], msg, strlen(msg));
}

// 读数据, 子线程检测的evLoop->socketPair[1]触发读事件，线程解除阻塞
void readLocalMessage(void* arg)
{
	struct EventLoop* evLoop = (struct EventLoop*)arg;
	char buf[256];
	read(evLoop->socketPair[1], buf, sizeof(buf));
	return 0;
}

struct EventLoop* eventLoopInitEx(const char* threadName)
{
	struct EventLoop* evLoop = (struct EventLoop*)malloc(sizeof(struct EventLoop));
	evLoop->isQuit = false;
	evLoop->dispatcher = &EpollDispatcher;
	evLoop->dispatcherData = evLoop->dispatcher->init();
	// 任务队列初始化
	evLoop->head = evLoop->tail = NULL;
	// 初始化ChannelMap
	evLoop->channelMap = channelMapInit(128);
	evLoop->threadID = pthread_self();
	strcpy(evLoop->threadName, threadName == NULL ? "MainThread" : threadName);
	pthread_mutex_init(&evLoop->mutex, NULL);
	int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, evLoop->socketPair);
	if (ret == -1)
	{
		perror("socketpair");
		exit(0);
	}
	// 指定规则: evLoop->socketPair[0] 发送数据, evLoop->socketPair[1] 接收数据
	struct Channel* channel = channelInit(evLoop->socketPair[1], ReadEvent, readLocalMessage, NULL, evLoop);
	// 把 channel 添加到任务队列
	eventLoopAddTask(evLoop, channel, ADD);
	return evLoop;
}

int eventLoopRun(struct EventLoop* evLoop)
{
	assert(evLoop != NULL);
	// 取出事件分发和检测模型
	struct Dispatcher* dispatcher = evLoop->dispatcher;
	// 判断线程ID是否正常
	if (evLoop->threadID != pthread_self())
		return -1;
	// 循环进行事件处理
	while (!evLoop->isQuit)
	{
		dispatcher->dispatch(evLoop, 2);		// 超时时长 2s
		eventLoopProcessTask(evLoop);
	}
	return 0;
}

int eventActivate(struct EventLoop* evLoop, int fd, int events)
{
	if (evLoop == NULL || fd < 0)
		return -1;

	// 取出 channel
	struct Channel* channel = evLoop->channelMap->list[fd];
	assert(channel->fd == fd);
	if ((events & ReadEvent) && (channel->readCallback != NULL))
	{
		channel->readCallback(channel->arg);
	}
	if ((events & WriteEvent) && (channel->writeCallback != NULL))
	{
		channel->writeCallback(channel->arg);
	}

	return 0;
}

int eventLoopAddTask(struct EventLoop* evLoop, struct Channel* channel, int type)
{
	// 加锁, 保护共享资源, 互斥访问任务队列
	pthread_mutex_lock(&(evLoop->mutex));
	// 创建新节点
	struct ChannelElement* node = (struct ChannelElement*)malloc(sizeof(struct ChannelElement));
	node->type = type;
	node->channel = channel;
	node->next = NULL;
	// 链表为空
	if (evLoop->head == NULL)
	{
		evLoop->head = evLoop->tail = node;
	}
	else
	{
		evLoop->tail->next = node;		// 添加
		evLoop->tail = node;			// 后移
	}
	pthread_mutex_unlock(&(evLoop->mutex));

	// 处理节点
	/*
	* 细节:
	*	1. 对于链表节点的添加: 可能是当前线程, 也有可能是其他线程(主线程)
	*		1). 修改fd的事件, 当前子线程发起并处理;
	*		2). 添加新的fd, 添加任务节点的操作由主线程发起;
	*	2. 不能让主线程处理任务队列，需要由当前的子线程去处理
	*/
	if (evLoop->threadID == pthread_self())
	{
		// 子线程
		eventLoopProcessTask(evLoop);
	}
	else
	{
		// 主线程 -- 通知子线程处理任务队列中的任务
		// 1. 子线程在工作; 2. 子线程被阻塞: select, poll, epoll
		taskWakeup(evLoop);
	}
	return 0;
}

int eventLoopProcessTask(struct EventLoop* evLoop)
{
	pthread_mutex_lock(&(evLoop->mutex));
	// 取出头节点
	struct ChannelElement* head = evLoop->head;
	while (head != NULL)
	{
		struct Channel* channel = head->channel;
		if (head->type == ADD)
		{
			// 添加
			eventLoopAdd(evLoop, channel);
		}
		else if (head->type == DELETE)
		{
			// 删除
			eventLoopRemove(evLoop, channel);
		}
		else if (head->type == MODIFY)
		{
			// 修改
			eventLoopModify(evLoop, channel);
		}
		struct ChannelElement* tmp = head;
		head = head->next;
		free(tmp);
	}
	evLoop->head = evLoop->tail = NULL;
	pthread_mutex_unlock(&(evLoop->mutex));
	return 0;
}

int eventLoopAdd(struct EventLoop* evLoop, struct Channel* channel)
{
	int fd = channel->fd;
	struct ChannelMap* channelMap = evLoop->channelMap;
	if (fd >= channelMap->size)
	{
		// 没有足够的空间存储键值对 fd -> channel ===> 扩容
		if (!makeMapRoom(channelMap, fd + 1, sizeof(struct Channel*)))
		{
			return -1;
		}
	}
	// 找到fd对应的数组元素位置, 并存储键值对 fd -> channel
	if (channelMap->list[fd] == NULL)
	{
		channelMap->list[fd] = channel;
		evLoop->dispatcher->add(channel, evLoop);
	}
	return 0;
}

int eventLoopRemove(struct EventLoop* evLoop, struct Channel* channel)
{
	int fd = channel->fd;
	struct ChannelMap* channelMap = evLoop->channelMap;
	if (fd >= channelMap->size)
	{
		return -1;
	}
	int ret = evLoop->dispatcher->remove(channel, evLoop);
	return ret;
}

int eventLoopModify(struct EventLoop* evLoop, struct Channel* channel)
{
	int fd = channel->fd;
	struct ChannelMap* channelMap = evLoop->channelMap;
	if (fd >= channelMap->size || channelMap->list[fd] == NULL)
	{
		return -1;
	}
	int ret = evLoop->dispatcher->modify(channel, evLoop);
	return ret;
}

int destroyChannel(struct EventLoop* evLoop, struct Channel* channel)
{
	int fd = channel->fd;
	struct ChannelMap* channelMap = evLoop->channelMap;
	// 删除 channel 和 fd 的对应关系
	channelMap->list[fd] = NULL;
	// 关闭文件描述符 fd
	close(fd);
	// 释放channel
	free(channel);
	return 0;
}
