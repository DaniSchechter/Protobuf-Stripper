#ifndef LOGGER_H_
#define LOGGER_H_

#define WHITE  "\x1B[39m"
#define RED    "\x1B[91m"
#define YELLOW "\x1B[93m"
#define BLUE   "\x1B[96m"
#define GREEN  "\x1B[32m"
#define RST    "\x1B[0m"

#include <iostream>
#include <mutex>

class Logger
{
public:
    enum class LOG_LEVEL
    {
        INFO, DEBUG, WARNING, FATAL
    };

    Logger() = delete;

    Logger(const Logger& logger) = delete;

    static void log(const std::string& message, LOG_LEVEL level);

private:
   
    static std::mutex output_stream_lock_ ;

};

#endif //LOGGER_H_