#include "ThreadPool.h"

ThreadPool::ThreadPool(int min, int max) : m_minThread(min), m_maxThread(max),
m_curThread(min), m_idleThread(min), m_stop(false)
{
	// �����������߳�
	m_manager = new std::thread()
}
