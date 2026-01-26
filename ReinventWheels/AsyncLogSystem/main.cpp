#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <atomic>
#include <sstream>
#include <vector>
#include <stdexcept>

// 辅助函数, 将单个参数转化为字符串
template<typename T>
std::string toStringHelper(T&& arg)
{
	std::ostringstream oss;
	oss << std::forward<T>(arg);
	return oss.str();
}

class LogQueue
{
public:
	void push(const std::string& msg)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(msg);
		m_condVar.notify_one();
	}

	bool pop(std::string& msg)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		/**
		 * 防止操作系统"虚假唤醒"消费者线程的方式:
		 * 
		 * 1.采用while循环的方式
		 *		while (m_queue.empty())
		 *		{
		 *			m_condVar.wait(lock);
		 *		}
		 * 
		 * 2.使用wait的第二个参数(返回值为bool类型的lambda表达式)。当消费者线程被唤醒之后, 若lambda表达式的返回值为false, 线程继续阻塞。
		 * 		m_condVar.wait(lock, [this]() {
		 *			return !m_queue.empty() || m_isShutdown;
		 *		});
		 */
		m_condVar.wait(lock, [this]() {
			return !m_queue.empty() || m_isShutdown;
		});

		// 消费逻辑
		if (m_isShutdown && m_queue.empty())
		{
			return false;
		}
	}

	void shutdown();

private:
	std::queue<std::string> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_condVar;
	bool m_isShutdown = false;
};