#pragma once

#include "Dispatcher.h"
#include <sys/select.h>	

class SelectDispatcher : public Dispatcher
{
public:
	explicit SelectDispatcher(EventLoop* evLoop = nullptr);
	~SelectDispatcher();
	int add(Channel* channel) override;			// 添加
	int remove(Channel* channel) override;		// 删除
	int modify(Channel* channel) override;		// 修改
	int dispatch(int timeout = 2) override;		// 事件检测(timeout的单位: s)

private:
	void setFdSet(struct Channel* channel);
	void clearFdSet(struct Channel* channel);

private:
	static const int MAX_SIZE = 1024;

	fd_set m_readSet;
	fd_set m_writeSet;
};