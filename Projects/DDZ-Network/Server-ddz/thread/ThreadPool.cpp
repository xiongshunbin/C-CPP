#include "ThreadPool.h"
#include <cstdlib>
#include <assert.h>

ThreadPool::ThreadPool(EventLoop* mainLoop, int count) : m_mainLoop(mainLoop), m_threadNum(count)
{
	m_workerThreads.clear();
}

ThreadPool::~ThreadPool()
{
	for (auto& item : m_workerThreads)
	{
		delete item;
	}
}

void ThreadPool::run()
{
	assert(!m_isStart);
	if (m_mainLoop->getThreadID() != std::this_thread::get_id())
	{
		exit(0);
	}
	m_isStart = true;
	if (m_threadNum > 0)
	{
		for (int i = 0; i < m_threadNum; ++i)
		{
			WorkerThread* subThread = new WorkerThread(i);
			subThread->run();
			m_workerThreads.push_back(subThread);
		}
	}
}

EventLoop* ThreadPool::takeWorkerEventLoop()
{
	assert(m_isStart);
	if (m_mainLoop->getThreadID() != std::this_thread::get_id())
	{
		exit(0);
	}
	// 从线程池中取出一个子线程，然后取出其中的反应堆实例
	EventLoop* evLoop = m_mainLoop;
	if (m_threadNum > 0)
	{
		evLoop = const_cast<EventLoop*>(m_workerThreads[m_index]->getEventLoop());
		m_index = (m_index + 1) % m_threadNum;
	}
	return evLoop;
}
