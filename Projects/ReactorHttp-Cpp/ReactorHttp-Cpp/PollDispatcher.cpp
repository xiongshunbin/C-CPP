#include "PollDispatcher.h"
#include <cstdio>
#include <cstdlib>

PollDispatcher::PollDispatcher(EventLoop* evLoop) : Dispatcher(evLoop)
{
	m_name = "Poll";
	m_maxfd = 0;
	m_fds = new struct pollfd[MAX_NODE];
	for (int i = 0; i < MAX_NODE; i++)
	{
		m_fds[i].fd = -1;
		m_fds[i].events = 0;
		m_fds[i].revents = 0;
	}
}

PollDispatcher::~PollDispatcher()
{
	delete[] m_fds;
}

int PollDispatcher::add(Channel* channel)
{
	int events = 0;
	if (channel->getEvent() & static_cast<int>(FDEvent::ReadEvent))
	{
		events |= POLLIN;
	}
	if (channel->getEvent() & static_cast<int>(FDEvent::WriteEvent))
	{
		events |= POLLOUT;
	}
	int i = 0;
	for (; i < MAX_NODE; i++)
	{
		if (m_fds[i].fd == -1)
		{
			m_fds[i].fd = channel->getSocketFd();
			m_fds[i].events = events;
			m_maxfd = i > m_maxfd ? i : m_maxfd;
			break;
		}
	}
	if (i >= MAX_NODE)
	{
		return -1;
	}
	return 0;
}

int PollDispatcher::remove(Channel* channel)
{
	int i = 0;
	for (; i < MAX_NODE; i++)
	{
		if (m_fds[i].fd == channel->getSocketFd())
		{
			m_fds[i].fd = -1;
			m_fds[i].events = 0;
			m_fds[i].revents = 0;
			break;
		}
	}
	// 通过 Channel 释放对应的 TcpConnection 资源
	channel->destroyCallback(const_cast<void*>(channel->getArg()));
	if (i >= MAX_NODE)
	{
		return -1;
	}
	return 0;
}

int PollDispatcher::modify(Channel* channel)
{
	int events = 0;
	if (channel->getEvent() & static_cast<int>(FDEvent::ReadEvent))
	{
		events |= POLLIN;
	}
	if (channel->getEvent() & static_cast<int>(FDEvent::WriteEvent))
	{
		events |= POLLOUT;
	}
	int i = 0;
	for (; i < MAX_NODE; i++)
	{
		if (m_fds[i].fd == channel->getSocketFd())
		{
			m_fds[i].events = events;
			break;
		}
	}
	if (i >= MAX_NODE)
	{
		return -1;
	}
	return 0;
}

int PollDispatcher::dispatch(int timeout)
{
	int count = poll(m_fds, m_maxfd + 1, timeout * 1000);
	if (count == -1)
	{
		perror("poll");
		exit(0);
	}
	for (int i = 0; i <= m_maxfd; i++)
	{
		if (m_fds[i].fd == -1)
		{
			continue;
		}

		if (m_fds[i].revents & POLLIN)
		{
			m_evLoop->eventActivate(m_fds[i].fd, static_cast<int>(FDEvent::ReadEvent));
		}
		if (m_fds[i].revents & POLLOUT)
		{
			m_evLoop->eventActivate(m_fds[i].fd, static_cast<int>(FDEvent::WriteEvent));
		}
	}
	return 0;
}
