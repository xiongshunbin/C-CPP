#include "ThreadPool.h"

ThreadPool::ThreadPool(int min, int max) : m_minThread(min), m_maxThread(max),
m_curThread(min), m_idleThread(min), m_stop(false)
{
	// 创建管理者线程
	m_manager = new std::thread()
}
