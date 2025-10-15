#pragma once

/**
 * C++实现高性能日志系统
 *      1.日志存储：文本文件
 *      2.日志内容：时间、级别、文件、行号、内容
 *      3.日志级别: DEBUG < INFO < WARN < ERROR < FATAL
 *      4.日志翻滚：设置日志的大小
 */

#include <string>
#include <fstream>
#include <mutex>
#include <atomic>

namespace utility {

#define debug(format, ...) \
    Logger::getInstance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define info(format, ...) \
    Logger::getInstance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define warn(format, ...) \
    Logger::getInstance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define error(format, ...) \
    Logger::getInstance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define fatal(format, ...) \
    Logger::getInstance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

class Logger
{
public:
    enum Level
    {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
        FATAL = 4,
        LEVEL_COUNT = 5
    };

    Logger(const Logger& logger) = delete;
    Logger& operator=(const Logger& logger) = delete;

    static Logger* getInstance();
    void openFile(const std::string& fileName);
    void closeFile();
    void log(Level level, const char* file, int line, const char* format, ...);
    void setLevel(Level level);
    void setMaxSize(int bytes);

private:
    Logger();
    ~Logger();
    void rotate();

private:
    std::string m_fileName;
    std::ofstream m_fout;
    Level m_level;
    int m_maxSize;      // 允许的日志文件大小, 如果为0, 默认都写入到一个文件中
    int m_curSize;      // 当前日志文件的大小
    static const char* s_level[LEVEL_COUNT];
    static std::mutex m_mutex;
    static std::atomic<Logger*> m_logger;
};

}