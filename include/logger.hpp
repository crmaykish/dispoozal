#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

enum LogLevel
{
    LOG_OFF,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

const LogLevel GLOBAL_LOG_LEVEL = LOG_INFO;

void Log(std::string message, LogLevel logLevel = LOG_INFO);

#endif // LOGGER_HPP
