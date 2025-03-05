#pragma once

#include "Channel.h"
#include "EventLoop.h"

struct Dispatcher
{
	void* (*init)();														// 初始化select, poll 或 epoll需要的数据块
	int (*add)(struct Channel* channel, struct EventLoop* evLoop);			// 添加
	int (*remove)(struct Channel* channel, struct EventLoop* evLoop);		// 删除
	int (*modify)(struct Channel* channel, struct EventLoop* evLoop);		// 修改
	int (*dispatch)(struct EventLoop* evLoop, int timeout);					// 事件检测(timeout的单位: s)
	int (*clear)(struct EventLoop* evLoop);									// 清除数据(关闭文件描述符 or 释放内存)
};