#pragma once

/**
 * C++ 异步日志系统
 *		1.支持格式化字符串输出
 *		2.支持日志等级
 * 
 */

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
#include <memory>
#include <fmt/format.h>

namespace utility {

#define IS_RELEASE 1

#if IS_RELEASE
	#define DEBUG(format, ...)
#else
	#define DEBUG(format, ...) \
	Logger::getInstance()->log(LogLevel::DEBUG, format, ##__VA_ARGS__)
#endif

#define INFO(format, ...) \
	Logger::getInstance()->log(LogLevel::INFO, format, ##__VA_ARGS__)

#define WARN(format, ...) \
	Logger::getInstance()->log(LogLevel::WARN, format, ##__VA_ARGS__)

#define ERROR(format, ...) \
	Logger::getInstance()->log(LogLevel::ERROR, format, ##__VA_ARGS__)

#define FATAL(format, ...) \
	Logger::getInstance()->log(LogLevel::FATAL, format, ##__VA_ARGS__)

// 辅助函数, 将单个参数转化为字符串
template<typename T>
std::string toStringHelper(T&& arg)
{
	std::ostringstream oss;
	oss << std::forward<T>(arg);
	return oss.str();
}

// 日志等级
enum class LogLevel
{
	DEBUG, INFO, WARN, ERROR, FATAL
};

class LogQueue
{
public:
	void push(const std::string& msg);
	bool pop(std::string& msg);
	void shutdown();

private:
	std::queue<std::string> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_condVar;
	bool m_isShutdown = false;
};

class Logger;
class SafeDeletor
{
public:
	void operator()(Logger* logger);
};

class Logger
{
	friend class SafeDeletor;

public:
	static std::shared_ptr<Logger> getInstance();

	void configure(const std::string& fileName);

	template<typename... Args>
	void log(LogLevel level, const std::string& format, Args&&... args)
	{
		std::string levelStr;
		switch (level)
		{
		case LogLevel::DEBUG:
			levelStr = "[DEBUG\t";		break;
		case LogLevel::INFO:
			levelStr = "[INFO\t";		break;
		case LogLevel::WARN:
			levelStr = "[WARN\t";		break;
		case LogLevel::ERROR:
			levelStr = "[ERROR\t";		break;
		case LogLevel::FATAL:
			levelStr = "[FATAL\t";		break;
		default:						break;
		}

		m_logQueue.push(levelStr + formatMessage(format, std::forward<Args>(args)...));
	}

private:
	Logger();
	~Logger();

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	std::string getCurrentTime();

	template<typename... Args>
	std::string formatMessage(const std::string& format, Args&&... args)
	{
		try
		{
			return getCurrentTime() + "] " + fmt::format(format, std::forward<Args>(args)...);
		}
		catch (const fmt::format_error& e)
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
	}

private:
	LogQueue m_logQueue;
	std::thread m_workerThread;
	std::ofstream m_logFile;
	std::atomic<bool> m_exitFlag;
	static std::shared_ptr<Logger> m_logger;
};

}	// namespace utility
