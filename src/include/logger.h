#pragma once
#include "lockqueue.h"
#include <string>

#define LOG_INFO(logmsgformat, ...) \
    do                              \
    {                               \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO);   \
        char c[1024] = {0};         \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c);              \
    } while (0)

#define LOG_ERROR(logmsgformat, ...)\
    do                              \
    {                               \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR);  \
        char c[1024] = {0};         \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c);              \
    } while (0)
    


enum LogLevel
{
    INFO,   // 普通信息
    ERROR,  // 错误信息
};

class Logger
{
public:
    // get single instance
    static Logger& GetInstance();
    // set level
    void SetLogLevel(LogLevel level);
    // write log
    void Log(std::string msg);
private:
    int logLevel_;
    LockQueue<std::string> lockQue_;

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

};
