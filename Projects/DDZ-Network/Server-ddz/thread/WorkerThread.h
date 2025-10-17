#pragma once

#include "EventLoop.h"
#include <condition_variable>

// 定义子线程对应的结构体
class WorkerThread
{
public:
	explicit WorkerThread(int index);
	~WorkerThread();

	// 启动线程
	void run();

	// 返回反应堆实例
	inline const EventLoop* getEventLoop() const { return m_evLoop; }

private:
	// 子线程的回调函数
	void subThreadRunning();

private:
	std::thread* m_thread = nullptr;		// 线程handle
	std::thread::id m_threadID;				// 线程ID
	std::string m_threadName;				// 线程名
	std::mutex m_mutex;						// 互斥锁, 用于线程同步
	std::condition_variable m_condition;	// 条件变量, 用于阻塞线程
	EventLoop* m_evLoop = nullptr;			// 反应堆模型
};
