#include "EpollDispatcher.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

EpollDispatcher::EpollDispatcher(EventLoop* evLoop) : Dispatcher(evLoop)
{
	m_name = "Epoll";
	m_epfd = epoll_create(10);
	if (m_epfd == -1)
	{
		perror("epoll_create");
		exit(0);
	}
	m_events = new struct epoll_event[MAX_NODE];
}

EpollDispatcher::~EpollDispatcher()
{
	close(m_epfd);
	delete[] m_events;
}

int EpollDispatcher::add(Channel* channel)
{
	int ret = epollCtl(channel, EPOLL_CTL_ADD);
	if (ret == -1)
	{
		perror("epoll_ctl add");
		exit(0);
	}
	return ret;
}

int EpollDispatcher::remove(Channel* channel)
{
	int ret = epollCtl(channel, EPOLL_CTL_DEL);
	if (ret == -1)
	{
		perror("epoll_ctl delete");
		exit(0);
	}
	// 通过 Channel 释放对应的 TcpConnection 资源
	channel->destroyCallback(const_cast<void*>(channel->getArg()));
	return ret;
}

int EpollDispatcher::modify(Channel* channel)
{
	int ret = epollCtl(channel, EPOLL_CTL_MOD);
	if (ret == -1)
	{
		perror("epoll_ctl modify");
		exit(0);
	}
	return ret;
}

int EpollDispatcher::dispatch(int timeout)
{
	int count = epoll_wait(m_epfd, m_events, MAX_NODE, timeout * 1000);
	for (int i = 0; i < count; i++)
	{
		int events = m_events[i].events;
		int fd = m_events[i].data.fd;
		if (events & EPOLLERR || events & EPOLLHUP)
		{
			// 对方断开了连接，删除文件描述符
			// epollRemove(Channel, evLoop);
			continue;
		}
		if (events & EPOLLIN)
		{
			m_evLoop->eventActivate(fd, static_cast<int>(FDEvent::ReadEvent));
		}
		if (events & EPOLLOUT)
		{
			m_evLoop->eventActivate(fd, static_cast<int>(FDEvent::WriteEvent));
		}
	}
	return 0;
}

int EpollDispatcher::epollCtl(Channel* channel, int op)
{
	struct epoll_event ev;
	ev.data.fd = channel->getSocketFd();
	int events = 0;
	if (channel->getEvent() & static_cast<int>(FDEvent::ReadEvent))
	{
		events |= EPOLLIN;
	}
	if (channel->getEvent() & static_cast<int>(FDEvent::WriteEvent))
	{
		events |= EPOLLOUT;
	}
	ev.events = events;
	int ret = epoll_ctl(m_epfd, op, channel->getSocketFd(), &ev);
	return ret;
}
