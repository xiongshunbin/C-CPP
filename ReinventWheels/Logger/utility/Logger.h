#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <atomic>

namespace utility {

#define DEBUG(format, ...) \
    Logger::getInstance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define INFO(format, ...) \
    Logger::getInstance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define WARN(format, ...) \
    Logger::getInstance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define ERROR(format, ...) \
    Logger::getInstance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define FATAL(format, ...) \
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
    void closeFile(const std::string fileName);
    void log(Level level, const char* file, int line, const char* format, ...);

private:
    Logger() = default;

private:
    std::string m_fileName;
    std::ofstream m_fout;
    static const char* s_level[LEVEL_COUNT];
    static std::mutex m_mutex;
    static std::atomic<Logger*> m_logger;
};

}