#ifndef A3_LOG_H
#define A3_LOG_H

#include <core/a3Settings.h>

enum a3LogLevel
{
    A3_LOG_LEVEL_BEGINBLOCK = -2,
    A3_LOG_LEVEL_ENDBLOCK = -1,
    A3_LOG_LEVEL_NONE = 0, // not display to the console
    A3_LOG_LEVEL_FATALERROR = 1,
    A3_LOG_LEVEL_ERROR = 2,
    A3_LOG_LEVEL_SERIOUSWARNING = 3,
    A3_LOG_LEVEL_WARNING = 4,
    A3_LOG_LEVEL_SUCCESS = 5,
    A3_LOG_LEVEL_INFO = 6,
    A3_LOG_LEVEL_DEV = 7,
    A3_LOG_LEVEL_DEBUG = 8,
    A3_LOG_LEVEL_ALLMESSAGES = 9 // Placeholder
};

// not support UTF8 log to file
class a3Log
{
public:
    static void log(a3LogLevel logLevel, const char* message, ...);

    // log to console / file
    static void fatalError(const char* message, ...);

    static void error(const char* message, ...);

    static void seriousWarning(const char* message, ...);

    static void warning(const char* message, ...);

    static void success(const char* message, ...);

    static void info(const char* message, ...);

    static void dev(const char* message, ...);

    static void debug(const char* message, ...);

private:
    a3Log() {}
    a3Log(const a3Log&) {}
    ~a3Log() {}
    a3Log& operator=(const a3Log& event) const {}

    static void log(a3LogLevel logLevel, const char* message, va_list args);
};


// --!自TattyUI中保留
// 函数调用error
#define a3FunctionError(errorMessage) a3FunctionError_<int>(std::string(__FUNCTION__), errorMessage)

#define a3FunctionErrorArg(argName, arg, errorMessage) a3FunctionError_(std::string(__FUNCTION__), argName, arg, errorMessage)

template<class T>
void a3FunctionError_(std::string functionName, std::string argName, T arg, std::string errorMessage)
{
    std::cout << "Error: " << "执行函数: " << functionName << "()时参数：" << argName << " = " << arg << "发生错误：" << errorMessage
        << std::endl;
}

template<class T>
void a3FunctionError_(std::string functionName, std::string errorMessage)
{
    std::cout << "Error: " << "执行函数: " << functionName << "()时发生错误：" << errorMessage << std::endl;
}

#endif