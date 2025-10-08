#pragma once

#include "Channel.h"
#include "EventLoop.h"
#include <string>

class EventLoop;

class Dispatcher
{
public:
	explicit Dispatcher(EventLoop* evLoop = nullptr);
	virtual ~Dispatcher();
	virtual int add(Channel* channel) = 0;			// 添加
	virtual int remove(Channel* channel) = 0;		// 删除
	virtual int modify(Channel* channel) = 0;		// 修改
	virtual int dispatch(int timeout = 2) = 0;		// 事件检测(timeout的单位: s)

protected:
	std::string m_name = std::string();
	EventLoop* m_evLoop = nullptr;
};