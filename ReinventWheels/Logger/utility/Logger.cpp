#include "Logger.h"

using namespace utility;

std::mutex Logger::m_mutex;
std::atomic<Logger*> Logger::m_logger;

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

    }
}
