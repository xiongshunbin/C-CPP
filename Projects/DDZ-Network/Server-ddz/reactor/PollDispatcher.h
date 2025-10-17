#pragma once

#include "Dispatcher.h"
#include <poll.h>

class PollDispatcher : public Dispatcher
{
public:
	explicit PollDispatcher(EventLoop* evLoop = nullptr);
	~PollDispatcher();
	int add(Channel* channel) override;			// 添加
	int remove(Channel* channel) override;		// 删除
	int modify(Channel* channel) override;		// 修改
	int dispatch(int timeout = 2) override;		// 事件检测(timeout的单位: s)

private:
	static const int MAX_NODE = 1024;

	int m_maxfd;
	struct pollfd* m_fds;
};