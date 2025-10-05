#pragma once

#include "Dispatcher.h"
#include <poll.h>

class PollDispatcher : public Dispatcher
{
public:
	explicit PollDispatcher(EventLoop* evLoop = nullptr);
	~PollDispatcher();
	int add(Channel* channel) override;			// ���
	int remove(Channel* channel) override;		// ɾ��
	int modify(Channel* channel) override;		// �޸�
	int dispatch(int timeout = 2) override;		// �¼����(timeout�ĵ�λ: s)

private:
	static const int MAX_NODE = 1024;

	int m_maxfd;
	struct pollfd* m_fds;
};