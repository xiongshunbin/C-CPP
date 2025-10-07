#include "SelectDispatcher.h"
#include <cstdio>
#include <cstdlib>

SelectDispatcher::SelectDispatcher(EventLoop* evLoop) : Dispatcher(evLoop)
{
	m_name = "Select";
	FD_ZERO(&(m_readSet));
	FD_ZERO(&(m_writeSet));
}

SelectDispatcher::~SelectDispatcher()
{
}

int SelectDispatcher::add(Channel* channel)
{
	if (channel->getSocketFd() >= MAX_SIZE)
	{
		return -1;
	}
	setFdSet(channel);
	return 0;
}

int SelectDispatcher::remove(Channel* channel)
{
	clearFdSet(channel);
	// 通过 Channel 释放对应的 TcpConnection 资源
	channel->destroyCallback(const_cast<void*>(channel->getArg()));
	return 0;
}

int SelectDispatcher::modify(Channel* channel)
{
	clearFdSet(channel);
	setFdSet(channel);
	return 0;
}

int SelectDispatcher::dispatch(int timeout)
{
	struct timeval val;
	val.tv_sec = timeout;
	val.tv_usec = 0;
	fd_set rdtmp = m_readSet;
	fd_set wrtmp = m_writeSet;
	int count = select(MAX_SIZE, &rdtmp, &wrtmp, NULL, &val);
	if (count == -1)
	{
		perror("select");
		exit(0);
	}
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (FD_ISSET(i, &rdtmp))
		{
			eventActivate(evLoop, i, ReadEvent);
		}

		if (FD_ISSET(i, &wrtmp))
		{
			eventActivate(evLoop, i, WriteEvent);
		}
	}
	return 0;
}

void SelectDispatcher::setFdSet(Channel* channel)
{
	if (channel->getEvent() & static_cast<int>(FDEvent::ReadEvent))
	{
		FD_SET(channel->getSocketFd(), &m_readSet);
	}
	if (channel->getEvent() & static_cast<int>(FDEvent::WriteEvent))
	{
		FD_SET(channel->getSocketFd(), &m_writeSet);
	}
}

void SelectDispatcher::clearFdSet(Channel* channel)
{
	if (channel->getEvent() & static_cast<int>(FDEvent::ReadEvent))
	{
		FD_CLR(channel->getSocketFd(), &m_readSet);
	}
	if (channel->getEvent() & static_cast<int>(FDEvent::WriteEvent))
	{
		FD_CLR(channel->getSocketFd(), &m_writeSet);
	}
}
