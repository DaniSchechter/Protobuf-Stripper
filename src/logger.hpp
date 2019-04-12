#ifndef LOGGER_H_
#define LOGGER_H_

#define WHITE  "\x1B[37m"
#define RED    "\x1B[31m"
#define YELLOW "\x1B[33m"
#define ORANGE "\x1b[101m"
#define RST    "\x1B[0m"

#include <iostream>
#include <ostream>
#include <vector>
#include <chrono>

class Logger
{
public:
    // todo give different streams sfor errors, and info together with warn
    enum class LOG_LEVEL
    {
        INFO, WARNING, FATAL
    };

    Logger() = delete;

    Logger(const Logger& logger) = delete;

    static void log(const std::string& message, LOG_LEVEL level);

    // TODO maybe delete
    static void log(const std::vector< const char* >& messages, LOG_LEVEL level);

private:

    //ostream& log_stream_;

};

#endif //LOGGER_H_