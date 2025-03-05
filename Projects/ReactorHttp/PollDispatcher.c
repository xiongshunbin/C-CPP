#include "Dispatcher.h"
#include <poll.h>

#define Max 1024

struct PollData
{
	int maxfd;
	struct pollfd fds[Max];
};

static void* pollInit()
{
	struct PollData* data = (struct PollData*)malloc(sizeof(struct PollData));
	data->maxfd = 0;
	for (int i = 0; i < Max; i++)
	{
		data->fds[i].fd = -1;
		data->fds[i].events = 0;
		data->fds[i].revents = 0;
	}
	return data;
}

static int pollAdd(struct Channel* channel, struct EventLoop* evLoop)
{
	struct PollData* data = (struct PollData*)(evLoop->dispatcherData);
	int events = 0;
	if (channel->events & ReadEvent)
		events |= POLLIN;
	if (channel->events & WriteEvent)
		events |= POLLOUT;
	int i = 0;
	for (; i < Max; i++)
	{
		if (data->fds[i].fd == -1)
		{
			data->fds[i].fd = channel->fd;
			data->fds[i].events = events;
			data->maxfd = data->maxfd < i ? i : data->maxfd;
			break;
		}
	}
	if (i >= Max)
		return -1;
	return 0;
}

static int pollRemove(struct Channel* channel, struct EventLoop* evLoop)
{
	struct PollData* data = (struct PollData*)(evLoop->dispatcherData);
	int i = 0;
	for (; i < Max; i++)
	{
		if (data->fds[i].fd == channel->fd)
		{
			data->fds[i].fd = -1;
			data->fds[i].events = 0;
			data->fds[i].revents = 0;
			break;
		}
	}
	if (i >= Max)
		return -1;
	return 0;
}

static int pollModify(struct Channel* channel, struct EventLoop* evLoop)
{
	struct PollData* data = (struct PollData*)(evLoop->dispatcherData);
	int events = 0;
	if (channel->events & ReadEvent)
		events |= POLLIN;
	if (channel->events & WriteEvent)
		events |= POLLOUT;
	int i = 0;
	for (; i < Max; i++)
	{
		if (data->fds[i].fd == channel->fd)
		{
			data->fds[i].events = events;
			break;
		}
	}
	if (i >= Max)
		return -1;
	return 0;
}

static int pollDispatch(struct EventLoop* evLoop, int timeout)
{
	struct PollData* data = (struct PollData*)(evLoop->dispatcherData);
	int count = poll(data->fds, data->maxfd + 1, timeout * 1000);
	if (count == -1)
	{
		perror("poll");
		exit(0);
	}
	for (int i = 0; i <= data->maxfd; i++)
	{
		if (data->fds[i].fd == -1)
			continue;
		if (data->fds[i].revents & POLLIN)
		{

		}
		if (data->fds[i].revents & POLLOUT)
		{

		}
	}
	return 0;
}

static int pollClear(struct EventLoop* evLoop)
{
	struct PollData* data = (struct PollData*)(evLoop->dispatcherData);
	free(data);
	return 0;
}

struct Dispatcher PollDispatcher = {
	pollInit,
	pollAdd,
	pollRemove,
	pollModify,
	pollDispatch,
	pollClear
};