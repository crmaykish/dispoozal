#include <iostream>
#include "logger.hpp"

void Log(std::string message, LogLevel logLevel)
{
    if (GLOBAL_LOG_LEVEL == LOG_OFF)
    {
        return;
    }

    if (!message.empty() && logLevel >= GLOBAL_LOG_LEVEL)
    {
        std::string level;
        switch (logLevel)
        {
        case LOG_OFF:
            break;
        case LOG_INFO:
            level = "INFO";
            break;
        case LOG_WARNING:
            level = "WARNING";
            break;
        case LOG_ERROR:
            level = "ERROR";
            break;
        }
        std::cout << level << " : " << message << std::endl;
    }
}