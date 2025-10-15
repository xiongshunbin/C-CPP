#include "Logger.h"
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <stdexcept>
#include <iostream>

using namespace utility;

std::mutex Logger::m_mutex;
std::atomic<Logger*> Logger::m_logger;

Logger::Logger() : m_level(DEBUG), m_curSize(0), 
    m_maxSize(0)
{

}

Logger::~Logger()
{
    closeFile();
}

const char* Logger::s_level[LEVEL_COUNT] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATA"
};

Logger* Logger::getInstance()
{
    Logger* logger = m_logger.load();
    if(logger == nullptr)
    {
        m_mutex.lock();
        logger = m_logger.load();
        if(logger == nullptr)
        {
            logger = new Logger;
            m_logger.store(logger);
        }
        m_mutex.unlock();
    }
    return logger;
}

void Logger::openFile(const std::string &fileName)
{
    m_fout.open(fileName, std::ios::app);
    if(m_fout.fail())
    {
        throw std::logic_error("Open file failed " + fileName);
    }
    m_fileName = fileName;
    m_fout.seekp(0, std::ios::end);
    m_curSize = m_fout.tellp();
}

void Logger::closeFile()
{
    m_fout.close();
}

void Logger::log(Level level, const char *file, int line, const char *format, ...)
{
    if (m_level > level)
    {
        return;
    }

    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);
    const char* fmt = "%s %s %s:%d ";
    int size = snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);
    if (size > 0)
    {
        char* buffer = new char[size + 1];
        snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
        buffer[size] = '\0';
        m_fout << buffer;
        m_curSize += size;
        delete [] buffer;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);
    size = vsnprintf(NULL, 0, format, arg_ptr);
    va_end(arg_ptr);
    if (size > 0)
    {
        char* content = new char[size + 1];
        va_start(arg_ptr, format);
        vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        content[size] = '\0';
        m_fout << content;
        m_curSize += size;
        delete [] content;
    }
    m_fout << "\n";
    m_fout.flush();

    if (m_curSize >= m_maxSize && m_maxSize > 0)
    {
        rotate();
    }
}

void Logger::setLevel(Level level)
{
    m_level = level;
}

void Logger::setMaxSize(int bytes)
{
    m_maxSize = bytes;
}

void Logger::rotate()
{
    closeFile();
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "_%Y-%m-%d_%H-%M-%S", ptm);
    std::string filePrefix = m_fileName.substr(0, m_fileName.rfind("."));
    std::string fileName = filePrefix + timestamp + ".log";
    if (rename(m_fileName.c_str(), fileName.c_str()) != 0)
    {
        throw std::logic_error("Rename log file failed: " + std::string(strerror(errno)));
    }
    openFile(m_fileName);
}
