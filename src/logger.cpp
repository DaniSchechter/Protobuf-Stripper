
#include "logger.hpp"
#include "server.hpp"

std::mutex Logger::output_stream_lock_;

void Logger::log(const std::string& message, Logger::LOG_LEVEL level)
{
    const char* COLOR = nullptr;
    switch(level)
    {
        case Logger::LOG_LEVEL::INFO:
            COLOR = WHITE;
            break;
        case Logger::LOG_LEVEL::WARNING:
            COLOR = YELLOW;
            break;
        case Logger::LOG_LEVEL::FATAL:
            COLOR = RED;
            break;
        case Logger::LOG_LEVEL::DEBUG:
            #ifdef DEBUG_MODE
                COLOR = BLUE;
            #else
                return;
            #endif            
    }

    Logger::output_stream_lock_.lock();
    std::cout << COLOR << message << RST << std::endl << std::endl;
    Logger::output_stream_lock_.unlock();

}

// void Logger::log(const std::vector< const char* >& messages, Logger::LOG_LEVEL level)
// {
//     switch(level)
//     {
//         case Logger::LOG_LEVEL::INFO:
//             std::cout << WHITE;
//             for(const char* message: messages)
//             {
//                 std::cout << message << " ";
//             }
//             std::cout << RST << std::endl;
//             break;
//         case Logger::LOG_LEVEL::WARNING:
//             std::cout << YELLOW;
//             for(const char* message: messages)
//             {
//                 std::cout << message << " ";
//             }
//             std::cout << RST << std::endl;
//             break;
//         case Logger::LOG_LEVEL::FATAL:
//             std::cerr << RED;
//             for(const char* message: messages)
//             {
//                 std::cerr << message << " ";
//             }
//             std::cerr << RST << std::endl;
//             break;
//     }
// }
