#pragma once

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
#include <chrono>
#include <iomanip>

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

		msg = m_queue.front();
		m_queue.pop();
		return true;
	}

	void shutdown()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_isShutdown = true;
		m_condVar.notify_all();
	}

private:
	std::queue<std::string> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_condVar;
	bool m_isShutdown = false;
};

enum class LogLevel
{
	INFO, DEBUG, WARN, ERROR
};

class Logger
{
public:
	Logger(const std::string& fileName) : m_logFile(fileName, std::ios::out | std::ios::app), m_exitFlag(false)
	{
		if (!m_logFile.is_open())
		{
			throw std::runtime_error("Failed to open log file!");
		}

		m_workerThread = std::thread([this]() {
			std::string msg;
			while (m_logQueue.pop(msg))
			{
				m_logFile << msg << std::endl;
			}
		});
	}
	~Logger()
	{
		m_exitFlag = true;
		m_logQueue.shutdown();
		if (m_workerThread.joinable())
		{
			m_workerThread.join();
		}

		if (m_logFile.is_open())
		{
			m_logFile.close();
		}
	}

	template<typename... Args>
	void log(LogLevel level, const std::string& format, Args&&... args)
	{
		std::string levelStr;
		switch (level)
		{
		case LogLevel::INFO:
			levelStr = "[INFO\t";		break;
		case LogLevel::DEBUG:
			levelStr = "[DEBUG\t";		break;
		case LogLevel::WARN:
			levelStr = "[WARN\t";		break;
		case LogLevel::ERROR:
			levelStr = "[ERROR\t";		break;
		default:						break;
		}

		m_logQueue.push(levelStr + formatMessage(format, std::forward<Args>(args)...));
	}

private:
	std::string getCurrentTime()
	{
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()) % 1000;
		std::tm buf;
#ifdef _WIN32
		localtime_s(&buf, &time); // Windows平台使用localtime_s
#else
		localtime_r(&time, &buf); // 非Windows平台使用localtime_r
#endif
		std::ostringstream oss;
		oss << std::put_time(&buf, "%y-%m-%d %T.") << std::setfill('0') << std::setw(3) << ms.count();
		return oss.str();
	}

	template<typename... Args>
	std::string formatMessage(const std::string& format, Args&&... args)
	{
		std::vector<std::string> argStrings = { toStringHelper(std::forward<Args>(args))... };
		std::ostringstream oss;
		size_t argIndex = 0;
		size_t pos = 0;
		size_t placeHolder = format.find("{}", pos);
		while (placeHolder != std::string::npos)
		{
			oss << format.substr(pos, placeHolder - pos);
			if (argIndex < argStrings.size())
			{
				oss << argStrings[argIndex++];
			}
			else
			{
				oss << "{}";
			}
			pos = placeHolder + 2;
			placeHolder = format.find("{}", pos);
		}

		oss << format.substr(pos);
		while (argIndex < argStrings.size())
		{
			oss << argStrings[argIndex++];
		}
		return getCurrentTime() + "] " + oss.str();
	}

	LogQueue m_logQueue;
	std::thread m_workerThread;
	std::ofstream m_logFile;
	std::atomic<bool> m_exitFlag;
};
