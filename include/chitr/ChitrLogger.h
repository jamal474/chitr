#ifndef CHITR_LOGGER_H
#define CHITR_LOGGER_H

#include <string>
#include <mutex>
#include <cstdarg>
#include <type_traits>

enum class LogLevel {
    DEBUG   = 0,
    INFO    = 1,
    WARN    = 2,
    ERROR   = 3,
    OFF     = 4
};

#ifndef CHITR_ACTIVE_LEVEL
    #define CHITR_ACTIVE_LEVEL 0
#endif

class ChitrLogger {
public:
    static ChitrLogger& getInstance();

    ChitrLogger(const ChitrLogger&) = delete;
    ChitrLogger& operator=(const ChitrLogger&) = delete;

    
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((format(printf, 5, 6)))
#endif

    void logInternal(LogLevel level, const char* file, int line, const char* format, ...);

    template<typename... Args>
    void log(LogLevel level, const char* file, int line, const char* format, Args... args) {
        logInternal(level, file, line, format, chitr_arg(args)...);
    }

private:
    ChitrLogger() = default;
    ~ChitrLogger() = default;
    
    std::mutex logMutex;
    std::string getCurrentTime();
    std::string getFileName(const char* filepath);

    static const char* chitr_arg(const std::string& s) {
        return s.c_str();
    }

    template<typename T>
    static T chitr_arg(T t) {
        return t;
    }
};


#if CHITR_ACTIVE_LEVEL <= 0
    #define LOG_DEBUG(...) ChitrLogger::getInstance().log(LogLevel::DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_DEBUG(...) void(0)
#endif

#if CHITR_ACTIVE_LEVEL <= 1
    #define LOG_INFO(...)  ChitrLogger::getInstance().log(LogLevel::INFO, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_INFO(...)  void(0)
#endif

#if CHITR_ACTIVE_LEVEL <= 2
    #define LOG_WARN(...)  ChitrLogger::getInstance().log(LogLevel::WARN, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_WARN(...)  void(0)
#endif

#if CHITR_ACTIVE_LEVEL <= 3
    #define LOG_ERROR(...) ChitrLogger::getInstance().log(LogLevel::ERROR, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_ERROR(...) void(0)
#endif

#endif