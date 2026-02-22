#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <vector>
#include <atomic>
#include <memory>

class ThreadPool
{
	using Task = std::packaged_task<void()>;
public:
	static ThreadPool& getInstance()
	{
		static ThreadPool instance;
		return instance;
	}

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	template<typename F, typename... Args>
	auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
	{
		using RetType = decltype(f(args...));
		if (this->_stop.load())
		{
			return std::future<RetType>();
		}

		auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		std::future<RetType> ret = task->get_future();
		{
			std::lock_guard<std::mutex> lock(_cv_mtx);
			_tasks.emplace([task]() {
				(*task)();
			});
		}
		_cv.notify_one();
		return ret;
	}

private:
	explicit ThreadPool(unsigned int num = std::thread::hardware_concurrency()) : _thread_num(num), _stop(false) 
	{
		start();
	}
	~ThreadPool()
	{
		stop();
	}

	void start()
	{
		for (int i = 0; i < _thread_num; ++i)
		{
			_pool.emplace_back([this]() {
				while (!this->_stop.load())
				{
					Task task;
					{
						std::unique_lock<std::mutex> lock(_cv_mtx);
						this->_cv.wait(lock, [this]() {
							return this->_stop.load() || !this->_tasks.empty();
						});
						if (this->_tasks.empty())
							return;

						task = std::move(this->_tasks.front());
						this->_tasks.pop();
					}
					this->_thread_num--;
					task();
					this->_thread_num++;
				}
			});
		}
	}

	void stop()
	{
		_stop.store(true);
		_cv.notify_all();
		for (auto& td : _pool)
		{
			if (td.joinable())
			{
				std::cout << "Join thread: " << td.get_id() << std::endl;
				td.join();
			}
		}
	}

private:
	std::mutex _cv_mtx;
	std::condition_variable _cv;
	std::atomic_int _thread_num;
	std::atomic_bool _stop;
	std::queue<Task> _tasks;
	std::vector<std::thread> _pool;
};
