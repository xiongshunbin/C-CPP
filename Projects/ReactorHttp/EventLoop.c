#include "EventLoop.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct EventLoop* eventLoopInit()
{
	return eventLoopInitEx(NULL);
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
