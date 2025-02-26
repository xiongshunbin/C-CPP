#include "ThreadPool.h"

#include <iostream>

ThreadPool::ThreadPool(int min, int max) : m_minThread(min), m_maxThread(max),
m_curThread(min), m_idleThread(min), m_stop(false)
{
	// 创建管理者线程
	m_manager = new std::thread(&ThreadPool::manager, this);

	// 创建工作者线程
	for (int i = 0; i < min; i++)
	{
		std::thread t(&ThreadPool::worker, this);
		m_workers.insert(std::make_pair(t.get_id(), std::move(t)));
	}
}

ThreadPool::~ThreadPool()
{
	m_stop = true;
	m_condition.notify_all();
	for (auto& it : m_workers)
	{
		std::thread& t = it.second;
		if (t.joinable())
		{
			std::cout << "********** Thread " << t.get_id() << " will exit......" << std::endl;
			t.join();
		}
	}

	if (m_manager->joinable())
	{
		m_manager->join();
	}
	delete m_manager;
}

void ThreadPool::addTask(std::function<void(void)> task)
{
	{
		std::lock_guard<std::mutex> locker(m_mutexQueue);
		m_taskQ.emplace(task);
	}

	m_condition.notify_one();
}

void ThreadPool::manager(void)
{
	while (!m_stop.load())
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));
		int idle = m_idleThread.load();
		int cur = m_curThread.load();
		if (idle > cur / 2 && cur > m_minThread)
		{
			// 每次销毁两个线程
			m_exitThread.store(2);
			m_condition.notify_all();
			std::lock_guard<std::mutex> lck(m_mutexIds);
			for (auto id : m_ids)
			{
				auto it = m_workers.find(id);
				if (it != m_workers.end())
				{
					std::cout << "==========The thread " << (*it).first << " is destroyed." << std::endl;
					(*it).second.join();
					m_workers.erase(it);
				}
			}
			m_ids.clear();
		}
		else if (idle == 0 && cur < m_maxThread)
		{
			std::thread t(&ThreadPool::worker, this);
			m_workers.insert(std::make_pair(t.get_id(), std::move(t)));
			std::cout << "The thread is created......" << std::endl;
			m_curThread++;
			m_idleThread++;
		}
	}
}

void ThreadPool::worker(void)
{
	while (!m_stop.load())
	{
		std::function<void(void)> task = nullptr;
		{
			std::unique_lock<std::mutex> locker(m_mutexQueue);
			while (m_taskQ.empty() && !m_stop)
			{
				m_condition.wait(locker);
				if (m_exitThread.load() > 0)
				{
					m_curThread--;
					m_idleThread--;
					m_exitThread--;
					std::cout << "----------The thread exited, ID: " << std::this_thread::get_id() << std::endl;
					std::lock_guard<std::mutex> lck(m_mutexIds);
					m_ids.emplace_back(std::this_thread::get_id());
					return;
				}
			}
			if (!m_taskQ.empty())
			{
				std::cout << "Took out a task......" << std::endl;
				task = std::move(m_taskQ.front());
				m_taskQ.pop();
			}
		}

		if (task)
		{
			m_idleThread--;
			task();
			m_idleThread++;
		}
	}
}

void calc(int x, int y)
{
	int z = x + y;
	std::cout << "z = " << z << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

int calc1(int x, int y)
{
	int z = x + y;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return z;
}

int main()
{
	ThreadPool pool;
	std::vector<std::future<int>> results;
	for (int i = 0; i < 10; i++)
	{
		//auto obj = std::bind(calc, i, i * 2);
		results.emplace_back(pool.addTask(calc1, i, i * 2));
	}
	for (auto& item : results)
	{
		std::cout << "The thread's results: " << item.get() << std::endl;
	}
	return 0;
}