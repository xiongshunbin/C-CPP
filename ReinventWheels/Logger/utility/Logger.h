#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <atomic>

namespace utility {

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

    Logger* getInstance();
    void log(Level level, const char* filePath, int line, const char* format, ...);

private:
    Logger() = default;

private:
    std::string m_filePath;
    std::ofstream m_fout;
    static const char* s_level[LEVEL_COUNT];
    static std::mutex m_mutex;
    static std::atomic<Logger*> m_logger;
};

}