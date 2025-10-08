#include "WorkerThread.h"
#include <cstdio>

WorkerThread::WorkerThread(int index)
{
	m_threadID = std::thread::id();
	m_threadName = "SubThread-" + std::to_string(index);
}

WorkerThread::~WorkerThread()
{
	if (m_thread != nullptr)
	{
		delete m_thread;
		m_thread = nullptr;
	}
}

void WorkerThread::run()
{
	// 创建子线程
	m_thread = new std::thread(&WorkerThread::subThreadRunning, this);
	// 阻塞主线程, 保证子线程成功创建反应堆模型
	std::unique_lock<std::mutex> locker(m_mutex);
	while (m_evLoop == nullptr)
	{
		m_condition.wait(locker);
	}
}

void WorkerThread::subThreadRunning()
{
	m_mutex.lock();
	m_evLoop = new EventLoop(m_threadName);
	m_mutex.unlock();
	m_condition.notify_one();
	m_evLoop->run();
}
