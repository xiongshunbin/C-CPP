#pragma once

#include <thread>
#include <vector>
#include <map>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <future>
#include <memory>
#include <condition_variable>


/*
* 构成：
*	1.管理者线程	->	子线程，1个
*		- 控制工作线程的数量：增加或减少
*	2.工作者线程	->	子线程，n个
*		- 从任务队列中取任务，并处理
*		- 若任务队列为空，条件变量阻塞
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

	// 添加任务	->	任务队列
	void addTask(std::function<void(void)> task);

	template <typename F, typename... Args>
	auto addTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
	{
		// 1. 创建package_task实例
		using returnType = typename std::result_of<F(Args...)>::type;
		auto mytask = std::make_shared<std::packaged_task<returnType()>> (
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);
		// 2. 得到future
		std::future<returnType> res = mytask->get_future();
		// 3. 任务函数添加到任务队列
		m_mutexQueue.lock();
		m_taskQ.emplace([mytask]() {
			(*mytask)();
			});
		m_mutexQueue.unlock();
		m_condition.notify_one();

		return res;
	}

private:
	void manager(void);
	void worker(void);

private:
	std::thread* m_manager;								// 管理者线程
	std::map<std::thread::id, std::thread> m_workers;	// 工作者线程
	std::vector<std::thread::id> m_ids;					// 退出线程函数的线程ID
	std::atomic<int> m_minThread;						// 最小线程数
	std::atomic<int> m_maxThread;						// 最大线程数
	std::atomic<int> m_curThread;						// 当前线程数
	std::atomic<int> m_idleThread;						// 空闲线程数
	std::atomic<int> m_exitThread;						// 退出线程数
	std::atomic<bool> m_stop;							// 线程池开关
	std::queue<std::function<void(void)>> m_taskQ;		// 任务队列
	std::mutex m_mutexQueue;							// 互斥锁，用于对任务队列的互斥访问
	std::mutex m_mutexIds;								// 互斥锁，用于对，m_ids的互斥访问
	std::condition_variable m_condition;				// 条件变量，当任务队列为空时，阻塞消费者进程
};

