#pragma once

#include "Dispatcher.h"

extern struct Dispatcher EpollDispatcher;
extern struct Dispatcher PollDispatcher;
extern struct Dispatcher SelectDispatcher;

struct EventLoop
{
	struct Dispatcher* dispatcher;
	void* dispatcherData;
};
