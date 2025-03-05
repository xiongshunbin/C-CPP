#include "Dispatcher.h"
#include <sys/select.h>
#include <stdio.h>

#define Max 1024

struct SelectData
{
	fd_set readSet;
	fd_set writeSet;
};

static void* selectInit()
{
	struct SelectData* data = (struct SelectData*)malloc(sizeof(struct SelectData));
	FD_ZERO(&(data->readSet));
	FD_ZERO(&(data->writeSet));
	return data;
}

static void setFdSet(struct Channel* channel, struct SelectData* data)
{
	if (channel->events & ReadEvent)
		FD_SET(channel->fd, &(data->readSet));
	if (channel->events & WriteEvent)
		FD_SET(channel->fd, &(data->writeSet));
}

static void clearFdSet(struct Channel* channel, struct SelectData* data)
{
	if (channel->events & ReadEvent)
		FD_CLR(channel->fd, &(data->readSet));
	if (channel->events & WriteEvent)
		FD_CLR(channel->fd, &(data->writeSet));
}

static int selectAdd(struct Channel* channel, struct EventLoop* evLoop)
{
	struct SelectData* data = (struct SelectData*)(evLoop->dispatcherData);
	if (channel->fd >= Max)
		return -1;
	setFdSet(channel, data);
	return 0;
}

static int selectRemove(struct Channel* channel, struct EventLoop* evLoop)
{
	struct SelectData* data = (struct SelectData*)(evLoop->dispatcherData);
	clearFdSet(channel, data);
	return 0;
}

static int selectModify(struct Channel* channel, struct EventLoop* evLoop)
{
	struct SelectData* data = (struct SelectData*)(evLoop->dispatcherData);
	if ((channel->events & ReadEvent) && !FD_ISSET(channel->fd, &(data->readSet)))
		FD_SET(channel->fd, &(data->readSet));
	else if (!(channel->events & ReadEvent) && FD_ISSET(channel->fd, &(data->readSet)))
		FD_CLR(channel->fd, &(data->readSet));
	else if ((channel->events & WriteEvent) && !FD_ISSET(channel->fd, &(data->writeSet)))
		FD_SET(channel->fd, &(data->writeSet));
	else if (!(channel->events & WriteEvent) && FD_ISSET(channel->fd, &(data->writeSet)))
		FD_CLR(channel->fd, &(data->writeSet));
	return 0;
}

static int selectDispatch(struct EventLoop* evLoop, int timeout)
{
	struct SelectData* data = (struct SelectData*)(evLoop->dispatcherData);
	struct timeval val;
	val.tv_sec = timeout;
	val.tv_usec = 0;
	fd_set rdtmp = data->readSet;
	fd_set wrtmp = data->writeSet;
	int count = select(Max, &rdtmp, &wrtmp, NULL, &val);
	if (count == -1)
	{
		perror("select");
		exit(0);
	}
	for (int i = 0; i < Max; i++)
	{
		if (FD_ISSET(i, &rdtmp))
		{

		}

		if (FD_ISSET(i, &wrtmp))
		{

		}
	}
	return 0;
}

static int selectClean(struct EventLoop* evLoop)
{
	struct SelectData* data = (struct SelectData*)(evLoop->dispatcherData);
	free(data);
}

struct Dispatcher SelectDispatcher = {
	selectInit,
	selectAdd,
	selectRemove,
	selectModify,
	selectDispatch,
	selectClean
};