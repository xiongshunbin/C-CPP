#pragma once

#include "Dispatcher.h"
#include "ChannelMap.h"
#include <pthread.h>

extern struct Dispatcher EpollDispatcher;
extern struct Dispatcher PollDispatcher;
extern struct Dispatcher SelectDispatcher;

// 处理节点中的channel的方式
enum ElemType
{
	ADD,
	DELETE,
	MODIFY
};

// 定义任务队列的结点
struct ChannelElement
{
	int type;							// 处理该节点中的channel的方式
	struct Channel* channel;
	struct ChannelElement* next;
};

struct EventLoop
{
	bool isQuit;						// 状态
	struct Dispatcher* dispatcher;
	void* dispatcherData;
	// 任务队列
	struct ChannelElement* head;
	struct ChannelElement* tail;
	// ChannelMap (文件描述符fd -> channel)
	struct ChannelMap* channelMap;
	pthread_t threadID;					// 线程ID
	char threadName[32];				// 线程名字
	pthread_mutex_t mutex;				// 互斥访问任务队列
	int socketPair[2];					// 存储本地通信的文件描述符, 通过socketpair初始化
};


// 初始化EventLoop
struct EventLoop* eventLoopInit();
struct EventLoop* eventLoopInitEx(const char* threadName);

// 启动反应堆模型
int eventLoopRun(struct EventLoop* evLoop);

// 处理被激活的文件描述符fd
int eventActivate(struct EventLoop* evLoop, int fd, int events);

// 添加任务到任务队列
int eventLoopAddTask(struct EventLoop* evLoop, struct Channel* channel, int type);

// 处理任务队列中的任务
int eventLoopProcessTask(struct EventLoop* evLoop);

// 处理dispatcher中的节点
int eventLoopAdd(struct EventLoop* evLoop, struct Channel* channel);
int eventLoopRemove(struct EventLoop* evLoop, struct Channel* channel);
int eventLoopModify(struct EventLoop* evLoop, struct Channel* channel);

// 释放channel
int destroyChannel(struct EventLoop* evLoop, struct Channel* channel);