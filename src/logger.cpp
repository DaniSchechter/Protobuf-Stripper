
#include "logger.hpp"
#include "server.hpp"

std::mutex Logger::output_stream_lock_;

void Logger::log(const std::string& message, Logger::LOG_LEVEL level)
{
    const char* COLOR = nullptr;
    switch(level)
    {
        case Logger::LOG_LEVEL::INFO:
            COLOR = BLUE;
            break;
        case Logger::LOG_LEVEL::WARNING:
            COLOR = YELLOW;
            break;
        case Logger::LOG_LEVEL::FATAL:
            COLOR = RED;
            break;
        case Logger::LOG_LEVEL::DEBUG:
            #ifdef DEBUG_MODE
                COLOR = WHITE;
            #else
                return;
            #endif
    }
    
    Logger::output_stream_lock_.lock();
    std::cout << COLOR << message << RST << std::endl << std::endl;
    Logger::output_stream_lock_.unlock();

}
