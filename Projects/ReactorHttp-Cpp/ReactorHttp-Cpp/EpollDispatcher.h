#pragma once

#include "Dispatcher.h"
#include <sys/epoll.h>

class EpollDispatcher : public Dispatcher
{
public:
	explicit EpollDispatcher(EventLoop* evLoop = nullptr);
	~EpollDispatcher();
	int add(Channel* channel) override;			// 添加
	int remove(Channel* channel) override;		// 删除
	int modify(Channel* channel) override;		// 修改
	int dispatch(int timeout = 2) override;		// 事件检测(timeout的单位: s)

private:
	int epollCtl(Channel* channel, int op);

private:
	static const int MAX_NODE = 520;

	int m_epfd;
	struct epoll_event* m_events;
};