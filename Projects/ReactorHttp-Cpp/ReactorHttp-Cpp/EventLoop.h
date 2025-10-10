#pragma once

#include "Dispatcher.h"
#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <string>

// 处理节点中的channel的方式
enum class ElemType : uint8_t
{
	ADD,
	DELETE,
	MODIFY
};

// 定义任务队列的结点
struct ChannelElement
{
	ElemType type;							// 处理该节点中的channel的方式
	Channel* channel;
};

class Dispatcher;

class EventLoop
{
public:
	EventLoop();
	explicit EventLoop(const std::string& threadName);
	~EventLoop();

	// 启动反应堆模型
	int run();

	// 处理被激活的文件描述符fd
	int eventActivate(int fd, int events);

	// 添加任务到任务队列
	int addTask(Channel* channel, ElemType type);

	// 处理任务队列中的任务
	int processTaskQueue();

	// 释放channel
	int freeChannel(Channel* channel);

	// 返回线程ID
	inline std::thread::id getThreadID() const { return m_threadID; }

private:
	static int readLocalMessage(void* arg);
	int readMessage();
	void taskWakeup();

	// 处理dispatcher中的节点
	int add(Channel* channel);
	int remove(Channel* channel);
	int modify(Channel* channel);

private:
	bool m_isQuit = true;					// 状态, 默认未启动
	// 父类指针指向子类的实例(SelectDispatcher, PollDispatcher, EpollDispatcher)
	Dispatcher* m_dispatcher = nullptr;
	// 任务队列
	std::queue<ChannelElement*> m_taskQueue;
	// ChannelMap (文件描述符fd -> channel)
	std::map<int, Channel*> m_channelMap;
	std::thread::id m_threadID;				// 线程ID
	std::string m_threadName;				// 线程名
	std::mutex m_mutex;						// 互斥访问任务队列
	int m_socketPair[2];					// 存储本地通信的文件描述符, 通过socketpair初始化
};
