#ifndef LOGGER_H_
#define LOGGER_H_

#define WHITE  "\x1B[39m"
#define RED    "\x1B[31m"
#define YELLOW "\x1B[33m"
#define ORANGE "\x1B[101m"
#define BLUE   "\x1B[34m"
#define RST    "\x1B[0m"

#include <iostream>
#include <ostream>
#include <vector>
#include <mutex>

class Logger
{
public:
    // todo give different streams sfor errors, and info together with warn
    enum class LOG_LEVEL
    {
        INFO, DEBUG, WARNING, FATAL
    };

    Logger() = delete;

    Logger(const Logger& logger) = delete;

    static void log(const std::string& message, LOG_LEVEL level);

private:
   
    static std::mutex output_stream_lock_ ;
    //ostream& log_stream_;

};

#endif //LOGGER_H_