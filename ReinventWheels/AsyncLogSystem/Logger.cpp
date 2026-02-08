#include "Logger.h"

namespace utility {

void LogQueue::push(const std::string& msg)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_queue.push(msg);
	m_condVar.notify_one();
}

bool LogQueue::pop(std::string& msg)
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

void LogQueue::shutdown()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_isShutdown = true;
	m_condVar.notify_all();
}

void SafeDeletor::operator()(Logger* logger)
{
	logger->log(LogLevel::INFO, "Ending application...");
	delete logger;
}

std::shared_ptr<Logger> Logger::m_logger = nullptr;

std::shared_ptr<Logger> Logger::getInstance()
{
	static std::once_flag flag;
	std::call_once(flag, []() {
		m_logger = std::shared_ptr<Logger>(new Logger(), SafeDeletor());
	});

	return m_logger;
}

Logger::Logger() : m_exitFlag(false)
{

}

Logger::~Logger()
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

std::string Logger::getCurrentTime()
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

void Logger::configure(const std::string& fileName)
{
	m_logFile.open(fileName, std::ios::out | std::ios::app);
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

	log(LogLevel::INFO, "Starting application...");
}

}	// namespace utility
