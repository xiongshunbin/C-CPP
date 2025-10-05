#pragma once

#include "Dispatcher.h"
#include <sys/epoll.h>

class EpollDispatcher : public Dispatcher
{
public:
	explicit EpollDispatcher(EventLoop* evLoop = nullptr);
	~EpollDispatcher();
	int add(Channel* channel) override;			// ���
	int remove(Channel* channel) override;		// ɾ��
	int modify(Channel* channel) override;		// �޸�
	int dispatch(int timeout = 2) override;		// �¼����(timeout�ĵ�λ: s)

private:
	int epollCtl(Channel* channel, int op);

private:
	static const int MAX_NODE = 520;

	int m_epfd;
	struct epoll_event* m_events;
};