// #ifndef LOGGER_H_
// #define LOGGER_H_

// #include <iostream>
// #include <fstream>
// #include <vector>

// class Logger
// {
// public:

//     enum class LOG_LEVEL
//     {
//         INFO, WARNING, FATAL
//     }

//     Logger() = delete;

//     Logger(const Logger& logger) = delete;

//     static void log(const string& message, LOG_LEVEL level) const;

//     static void log(const std::vector<string>& message, LOG_LEVEL level) const;

// private:

//     ostream & log_stream_;

// }

// #endif //LOGGER_H_