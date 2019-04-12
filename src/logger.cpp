
#include "logger.hpp"

void Logger::log(const std::string& message, Logger::LOG_LEVEL level)
{
    switch(level)
    {
        case Logger::LOG_LEVEL::INFO:
            std::cout << WHITE << message;
            break;
        case Logger::LOG_LEVEL::WARNING:
            std::cout << YELLOW << message;
            break;
        case Logger::LOG_LEVEL::FATAL:
            std::cout << RED << message;
            break;
    }
    std::cout << RST << std::endl << std::endl;
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
