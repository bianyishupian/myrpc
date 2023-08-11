#include <time.h>
#include <iostream>

#include "logger.h"

// get single instance
Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}
// set level
void Logger::SetLogLevel(LogLevel level)
{
    logLevel_ = level;
}
// write log
void Logger::Log(std::string msg)
{
    lockQue_.Push(msg);
}

Logger::Logger()
{
    std::thread writeLogTask([&](){
        for(;;)
        {
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);

            FILE *f = fopen(file_name, "a+");
            if(f == nullptr)
            {
                std::cout << "logger file : " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = lockQue_.Pop();

            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d =>[%s] ",
                    nowtm->tm_hour,
                    nowtm->tm_min,
                    nowtm->tm_sec,
                    (logLevel_ == INFO ? "info" : "error"));
            msg.insert(0, time_buf);
            msg.append("\n");

            fputs(msg.c_str(), f);
            fclose(f);
        }
    });

    writeLogTask.detach();
}