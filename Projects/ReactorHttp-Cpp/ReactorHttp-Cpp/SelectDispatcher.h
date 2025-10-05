#pragma once

#include "Dispatcher.h"
#include <sys/select.h>	

class SelectDispatcher : public Dispatcher
{
public:
	explicit SelectDispatcher(EventLoop* evLoop = nullptr);
	~SelectDispatcher();
	int add(Channel* channel) override;			// ���
	int remove(Channel* channel) override;		// ɾ��
	int modify(Channel* channel) override;		// �޸�
	int dispatch(int timeout = 2) override;		// �¼����(timeout�ĵ�λ: s)

private:
	void setFdSet(struct Channel* channel);
	void clearFdSet(struct Channel* channel);

private:
	static const int MAX_SIZE = 1024;

	fd_set m_readSet;
	fd_set m_writeSet;
};