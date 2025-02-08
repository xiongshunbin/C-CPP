#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>s

/*
* 构成：
*	1.管理者线程	->	子线程，1个
*		- 控制工作线程的数量：增加或减少
*	2.工作者线程	->	子线程，n个
*		- 从任务队列中取任务，并处理
*		- 若任务队列为空，被阻塞（被条件变量阻塞）
*		- 线程同步（互斥锁）
*		- 最小、最大、当前、空闲线程数量
*	3.任务队列	->	STL	-> queue
*		- 互斥锁
*		- 条件变量
*	4. 线程池开关	-> bool
*/
class ThreadPool
{
public:
	ThreadPool(int min = 2, int max = std::thread::hardware_concurrency());
	~ThreadPool();

private:
	std::thread* m_manager;
	std::vector<std::thread> m_workers;
	std::atomic<int> m_minThread;
	std::atomic<int> m_maxThread;
	std::atomic<int> m_curThread;
	std::atomic<int> m_idleThread;
	std::atomic<bool> m_stop;
	std::queue<std::function<void(void)>> m_task;
	std::mutex m_mutexQueue;
	std::condition_variable m_condition;
};

