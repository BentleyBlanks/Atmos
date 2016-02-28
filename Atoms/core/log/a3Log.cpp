#include <core/log/a3Log.h>
#include <stddef.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strstream>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream> 
#include <fstream>
#include <iomanip>
#include <core/a3Settings.h>

#ifdef A3_PLATFORM_WINDOWS
#define NOMINMAX 
#include <Windows.h>
#endif

// not supported UTF8
//#define A3_LOG_UTF8
//
//a3Log::a3Log()/* : level(A3_LOG_LEVEL_ALLMESSAGES)*/
//{
//
//}

//a3Log* a3Log::getInstance()
//{
//    static a3Log temp;
//    return &temp;
//}

void a3Log::log(a3LogLevel logLevel, const char* message, va_list args)
{
    //a3Log* gLog = a3Log::getInstance();
    //a3LogLevel level = gLog->level;

    const char* lv;
    time_t cur;
    const struct tm* tm;

    //if(logLevel > level)
    //    return;

    switch(logLevel)
    {
    case A3_LOG_LEVEL_FATALERROR: lv = "Fatal Error"; break;
    case A3_LOG_LEVEL_ERROR: lv = "Error"; break;
    case A3_LOG_LEVEL_SERIOUSWARNING: lv = "Serious Warning"; break;
    case A3_LOG_LEVEL_WARNING: lv = "Warning"; break;
    case A3_LOG_LEVEL_SUCCESS: lv = "Success"; break;
    case A3_LOG_LEVEL_INFO: lv = "Info"; break;
    case A3_LOG_LEVEL_DEV: lv = "Dev"; break;
    case A3_LOG_LEVEL_DEBUG: lv = "Debug"; break;
    default: lv = "Other"; break;
    }

    cur = time(NULL);
    tm = localtime(&cur);

#ifdef _MSC_VER
        WORD attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        switch(logLevel)
        {
        case A3_LOG_LEVEL_SUCCESS: attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;

        case A3_LOG_LEVEL_FATALERROR:
        case A3_LOG_LEVEL_ERROR: attr = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;

        case A3_LOG_LEVEL_SERIOUSWARNING:
        case A3_LOG_LEVEL_WARNING: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;

        case A3_LOG_LEVEL_INFO: attr = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;

        case A3_LOG_LEVEL_DEV:
        case A3_LOG_LEVEL_DEBUG: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case A3_LOG_LEVEL_ALLMESSAGES: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;

        default:
            break;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), attr);

        static char contentBuffer[1024];
        char* content = NULL;

        // save time + message + format(args) to content
        static char timeBuffer[30], messageBuffer[1024];
        sprintf(timeBuffer, "%02d:%02d:%02d    %s    ", tm->tm_hour, tm->tm_min, tm->tm_sec, lv);
        // messageBuffer:[hour]:[minute]:[second]
        strcpy(messageBuffer, timeBuffer);
        // messageBuffer:[hour]:[minute]:[second] message ...
        strcat(messageBuffer, message);
        // messageBuffer:[hour]:[minute]:[second] message'
        vsprintf(contentBuffer, messageBuffer, args);
        // allocate true content array
        int t = strlen(contentBuffer);
        content = new char[t + 1];
        strncpy(content, contentBuffer, t);
        content[t] = '\0';

        vprintf(content, args);

#ifdef A3_LOG_UTF8
        {
            char cache[16 * 1024 + 32] = {'\0'};
            wchar_t wcache[16 * 1024 + 32] = {L'\0'};
            int sz = vsnprintf(cache, 16 * 1024, message, args);

            if(sz >= 0)
                cache[sz] = '\0';
            else
                sz = sprintf(cache, "ks::SLog vsnprintf() return %d\n", sz);

            sz = MultiByteToWideChar(CP_UTF8, 0, cache, sz, wcache, 16 * 1024);
            if(sz > 0)
            {
                if(sz > 16 * 1024)
                    sz = 16 * 1024;
                wcache[sz] = L'\0';
            }
            else
            {
                wsprintfW(wcache, L"ks::SLog MultiByteToWideChar(CP_UTF8) return %d", sz);
            }

            // bad luck, chinese charactor show as ??????
            // fwprintf(stderr, L"%02d:%02d:%02d\t%S\t%s", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, wcache);

            sz = WideCharToMultiByte(0, 0, wcache, -1, cache, 16 * 1024, NULL, NULL);
            if(sz >= 0)
                cache[sz] = '\0';
            else
                sz = sprintf(cache, "ks::SLog WideCharToMultiByte(0) return %d\n", sz);

            fprintf(stderr, "%02d:%02d:%02d\t%s\t%s", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, cache);
            fflush(stderr);
        }
#endif
#else
    // linux console color if need
    if(slog_to_console)
    {
        va_list temp_args;
        va_copy(temp_args, args);

        fprintf(stderr, "%02d:%02d:%02d\t%s\t", tm->tm_hour, tm->tm_min, tm->tm_sec, lv);
        vfprintf(stderr, fmt, temp_args);
        fflush(stderr);

        va_end(temp_args);
    }
#endif
}

void a3Log::log(a3LogLevel logLevel, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(logLevel, message, args);
    va_end(args);
}

//void a3Log::outputToConsole(bool outputToConsole)
//{
//    logToConsole = outputToConsole;
//}

//a3LogLevel a3Log::getLogLevel()
//{
//    return level;
//}
//
//void a3Log::setLogLevel(a3LogLevel logLevel)
//{
//    if(level != A3_LOG_LEVEL_NONE)
//        level = logLevel;
//}

void a3Log::fatalError(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_FATALERROR, message, args);
    va_end(args);
}

void a3Log::error(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_ERROR, message, args);
    va_end(args);
}

void a3Log::seriousWarning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SERIOUSWARNING, message, args);
    va_end(args);
}

void a3Log::warning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_WARNING, message, args);
    va_end(args);
}

void a3Log::success(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SUCCESS, message, args);
    va_end(args);
}

void a3Log::info(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_INFO, message, args);
    va_end(args);
}

void a3Log::dev(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEV, message, args);
    va_end(args);
}

void a3Log::debug(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEBUG, message, args);
    va_end(args);
}