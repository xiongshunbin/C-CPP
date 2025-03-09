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

// д����, ���߳�ͨ����evLoop->socketPair[0]д���ݻ������߳�
void taskWakeup(struct EventLoop* evLoop)
{
	const char* msg = "Hello World!";
	write(evLoop->socketPair[0], msg, strlen(msg));
}

// ������, ���̼߳���evLoop->socketPair[1]�������¼����߳̽������
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
	// ������г�ʼ��
	evLoop->head = evLoop->tail = NULL;
	// ��ʼ��ChannelMap
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
	// ָ������: evLoop->socketPair[0] ��������, evLoop->socketPair[1] ��������
	struct Channel* channel = channelInit(evLoop->socketPair[1], ReadEvent, readLocalMessage, NULL, evLoop);
	// �� channel ��ӵ��������
	eventLoopAddTask(evLoop, channel, ADD);
	return evLoop;
}

int eventLoopRun(struct EventLoop* evLoop)
{
	assert(evLoop != NULL);
	// ȡ���¼��ַ��ͼ��ģ��
	struct Dispatcher* dispatcher = evLoop->dispatcher;
	// �ж��߳�ID�Ƿ�����
	if (evLoop->threadID != pthread_self())
		return -1;
	// ѭ�������¼�����
	while (!evLoop->isQuit)
	{
		dispatcher->dispatch(evLoop, 2);		// ��ʱʱ�� 2s
		eventLoopProcessTask(evLoop);
	}
	return 0;
}

int eventActivate(struct EventLoop* evLoop, int fd, int events)
{
	if (evLoop == NULL || fd < 0)
		return -1;

	// ȡ�� channel
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
	// ����, ����������Դ, ��������������
	pthread_mutex_lock(&(evLoop->mutex));
	// �����½ڵ�
	struct ChannelElement* node = (struct ChannelElement*)malloc(sizeof(struct ChannelElement));
	node->type = type;
	node->channel = channel;
	node->next = NULL;
	// ����Ϊ��
	if (evLoop->head == NULL)
	{
		evLoop->head = evLoop->tail = node;
	}
	else
	{
		evLoop->tail->next = node;		// ���
		evLoop->tail = node;			// ����
	}
	pthread_mutex_unlock(&(evLoop->mutex));

	// ����ڵ�
	/*
	* ϸ��:
	*	1. ��������ڵ�����: �����ǵ�ǰ�߳�, Ҳ�п����������߳�(���߳�)
	*		1). �޸�fd���¼�, ��ǰ���̷߳��𲢴���;
	*		2). ����µ�fd, �������ڵ�Ĳ��������̷߳���;
	*	2. ���������̴߳���������У���Ҫ�ɵ�ǰ�����߳�ȥ����
	*/
	if (evLoop->threadID == pthread_self())
	{
		// ���߳�
		eventLoopProcessTask(evLoop);
	}
	else
	{
		// ���߳� -- ֪ͨ���̴߳�����������е�����
		// 1. ���߳��ڹ���; 2. ���̱߳�����: select, poll, epoll
		taskWakeup(evLoop);
	}
	return 0;
}

int eventLoopProcessTask(struct EventLoop* evLoop)
{
	pthread_mutex_lock(&(evLoop->mutex));
	// ȡ��ͷ�ڵ�
	struct ChannelElement* head = evLoop->head;
	while (head != NULL)
	{
		struct Channel* channel = head->channel;
		if (head->type == ADD)
		{
			// ���
			eventLoopAdd(evLoop, channel);
		}
		else if (head->type == DELETE)
		{
			// ɾ��
			eventLoopRemove(evLoop, channel);
		}
		else if (head->type == MODIFY)
		{
			// �޸�
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
		// û���㹻�Ŀռ�洢��ֵ�� fd -> channel ===> ����
		if (!makeMapRoom(channelMap, fd + 1, sizeof(struct Channel*)))
		{
			return -1;
		}
	}
	// �ҵ�fd��Ӧ������Ԫ��λ��, ���洢��ֵ�� fd -> channel
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
	// ɾ�� channel �� fd �Ķ�Ӧ��ϵ
	channelMap->list[fd] = NULL;
	// �ر��ļ������� fd
	close(fd);
	// �ͷ�channel
	free(channel);
	return 0;
}
