#include <core/log/a3LogFile.h>
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

std::ofstream* a3LogFile::ofile = new std::ofstream();
std::ifstream* a3LogFile::ifile = new std::ifstream();

// not supported UTF8
//#define A3_LOG_UTF8

//a3LogFile::a3LogFile()/* : level(A3_LOG_LEVEL_ALLMESSAGES)*/
//{
//
//}

//a3LogFile* a3LogFile::getInstance()
//{
//    static a3LogFile temp;
//    return &temp;
//}

void a3LogFile::log(a3LogLevel logLevel, const char* prefix, const char* suffix, const char* message, va_list args)
{
    //a3LogFile* gLog = a3LogFile::getInstance();
    //a3LogLevel level = gLog->level;
    //std::ofstream* ofile = gLog->file;
    //std::ifstream* ifile = gLog->ifile;

    const char* lv;
    time_t cur;
    const struct tm* tm;

    //if(logLevel > level)
    //    return;

    switch(logLevel)
    {
    case A3_LOG_LEVEL_FATALERROR: lv = "System"; break;
    case A3_LOG_LEVEL_ERROR: lv = "Error"; break;
    case A3_LOG_LEVEL_SERIOUSWARNING: lv = "SeriousWarning"; break;
    case A3_LOG_LEVEL_WARNING: lv = "Warning"; break;
    case A3_LOG_LEVEL_SUCCESS: lv = "Success"; break;
    case A3_LOG_LEVEL_INFO: lv = "Info"; break;
    case A3_LOG_LEVEL_DEV: lv = "Dev"; break;
    case A3_LOG_LEVEL_DEBUG: lv = "Debug"; break;
    default: lv = "Other"; break;
    }

    cur = time(NULL);
    tm = localtime(&cur);

#ifdef A3_PLATFORM_WINDOWS
#ifdef A3_LOG_UTF8
    char cache[16 * 1024 + 32] = {'\0'};
    wchar_t wcache[16 * 1024 + 32] = {'\0'};
    int sz = vsnprintf(cache, 16 * 1024, message, args);

    if(sz >= 0)
        cache[sz] = '\0';
    else
        sz = sprintf(cache, "a3Log::vsnprintf() return %d\n", sz);

    sz = MultiByteToWideChar(CP_UTF8, 0, cache, sz, wcache, 16 * 1024);
    if(sz > 0)
    {
        if(sz > 16 * 1024)
            sz = 16 * 1024;
        wcache[sz] = L'\0';
    }
    else
        wsprintfW(wcache, L"a3Log::MultiByteToWideChar(CP_UTF8) return %d", sz);

    sz = WideCharToMultiByte(0, 0, wcache, -1, cache, 16 * 1024, NULL, NULL);
    if(sz >= 0)
        cache[sz] = '\0';
    else
        sz = sprintf(cache, "a3Log::WideCharToMultiByte(0) return %d\n", sz);

    fprintf(stderr, "%02d:%02d:%02d\t%s\t%s%s%s", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, prefix, cache, suffix);

    fflush(stderr);
#else 
#endif

#else
    // linux console color if need
    va_list temp_args;
    va_copy(temp_args, args);

    fprintf(stderr, "%02d:%02d:%02d\t%s\t%s\t", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, prefix);
    vfprintf(stderr, fmt, temp_args);
    fprintf(stderr, "\t%s", suffix);

    va_end(temp_args);
#endif

    // 低效 写的烂……
    // save to file
    if(ofile != NULL)
    {
        static char name[128], contentBuffer[1024];
        char* content = NULL;
        //sprintf(name, ".\\%02d-%02d-%02d-%s-%s.%s", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, prefix, suffix);
        sprintf(name, "%02d-%02d-%02d-%s.%s", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, prefix, suffix);

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

        // read content from the file
        char *buffer = NULL;
        ifile->open(name);
        if(ifile->is_open())
        {
            ifile->ignore(std::numeric_limits<std::streamsize>::max());
            std::streamsize length = ifile->gcount();
            // go back to the beginning  
            ifile->seekg(0, std::ios::beg);
            buffer = new char[length + 1];
            ifile->read(buffer, length);
            buffer[length] = '\0';
            ifile->close();
        }

        // add content to file
        ofile->open(name);
        if(buffer)
            *ofile << buffer << content << std::endl;
        else
            *ofile << content << std::endl;
        ofile->close();

        //delete content;
        if(buffer)
            delete[] buffer;
    }
}

void a3LogFile::log(a3LogLevel logLevel, const char* message, va_list args)
{
    //a3LogFile* gLog = a3LogFile::getInstance();
    //a3LogLevel level = gLog->level;
    //std::ofstream* ofile = gLog->file;
    //std::ifstream* ifile = gLog->ifile;

    const char* lv;
    time_t cur;
    const struct tm* tm;

    //if(logLevel > level)
    //    return;

    switch(logLevel)
    {
    case A3_LOG_LEVEL_FATALERROR: lv = "System"; break;
    case A3_LOG_LEVEL_ERROR: lv = "Error"; break;
    case A3_LOG_LEVEL_SERIOUSWARNING: lv = "SeriousWarning"; break;
    case A3_LOG_LEVEL_WARNING: lv = "Warning"; break;
    case A3_LOG_LEVEL_SUCCESS: lv = "Success"; break;
    case A3_LOG_LEVEL_INFO: lv = "Info"; break;
    case A3_LOG_LEVEL_DEV: lv = "Dev"; break;
    case A3_LOG_LEVEL_DEBUG: lv = "Debug"; break;
    default: lv = "Other"; break;
    }

    cur = time(NULL);
    tm = localtime(&cur);

#ifdef A3_PLATFORM_WINDOWS
#ifdef A3_LOG_UTF8
    char cache[16 * 1024 + 32] = {'\0'};
    wchar_t wcache[16 * 1024 + 32] = {'\0'};
    int sz = vsnprintf(cache, 16 * 1024, message, args);

    if(sz >= 0)
        cache[sz] = '\0';
    else
        sz = sprintf(cache, "a3Log::vsnprintf() return %d\n", sz);

    sz = MultiByteToWideChar(CP_UTF8, 0, cache, sz, wcache, 16 * 1024);
    if(sz > 0)
    {
        if(sz > 16 * 1024)
            sz = 16 * 1024;
        wcache[sz] = L'\0';
    }
    else
        wsprintfW(wcache, L"a3Log::MultiByteToWideChar(CP_UTF8) return %d", sz);

    sz = WideCharToMultiByte(0, 0, wcache, -1, cache, 16 * 1024, NULL, NULL);
    if(sz >= 0)
        cache[sz] = '\0';
    else
        sz = sprintf(cache, "a3Log::WideCharToMultiByte(0) return %d\n", sz);

    fprintf(stderr, "%02d:%02d:%02d\t%s\t%s%s%s", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, prefix, cache, suffix);

    fflush(stderr);
#else 
#endif

#else
    // linux console color if need
    va_list temp_args;
    va_copy(temp_args, args);

    fprintf(stderr, "%02d:%02d:%02d\t%s\t%s\t", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, prefix);
    vfprintf(stderr, fmt, temp_args);
    fprintf(stderr, "\t%s", suffix);

    va_end(temp_args);
#endif

    // 低效 写的烂……
    // save to file
    if(ofile != NULL)
    {
        static char name[128], contentBuffer[1024];
        char* content = NULL;
        //sprintf(name, ".\\%02d-%02d-%02d-%s-%s.%s", tm->tm_hour, tm->tm_min, tm->tm_sec, lv, prefix, suffix);

        sprintf(name, "%02d-%02d-%02d-Atoms-%s.log", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, atomsVersion.get().c_str());

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

        // read content from the file
        char *buffer = NULL;
        ifile->open(name);
        if(ifile->is_open())
        {
            ifile->ignore(std::numeric_limits<std::streamsize>::max());
            std::streamsize length = ifile->gcount();
            // go back to the beginning  
            ifile->seekg(0, std::ios::beg);
            buffer = new char[length + 1];
            ifile->read(buffer, length);
            buffer[length] = '\0';
            ifile->close();
        }

        // add content to file
        ofile->open(name);
        if(buffer)
            *ofile << buffer << content << std::endl;
        else
            *ofile << content << std::endl;
        ofile->close();

        //delete content;
        if(buffer)
            delete[] buffer;
    }
}

void a3LogFile::log(a3LogLevel logLevel, const char* prefix, const char* suffix, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(logLevel, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::log(a3LogLevel logLevel, const char* message, ...)
{

}

//a3LogLevel a3LogFile::getLogLevel()
//{
//    return level;
//}
//
//void a3LogFile::setLogLevel(a3LogLevel logLevel)
//{
//    if(level != A3_LOG_LEVEL_NONE)
//        level = logLevel;
//}

void a3LogFile::fatalError(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_FATALERROR, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::fatalError(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_FATALERROR, message, args);
    va_end(args);
}

void a3LogFile::error(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_ERROR, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::error(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_ERROR, message, args);
    va_end(args);
}

void a3LogFile::seriousWarning(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SERIOUSWARNING, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::seriousWarning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SERIOUSWARNING, message, args);
    va_end(args);
}

void a3LogFile::warning(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_WARNING, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::warning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_WARNING, message, args);
    va_end(args);
}

void a3LogFile::success(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SUCCESS, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::success(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SUCCESS, message, args);
    va_end(args);
}

void a3LogFile::info(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_INFO, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::info(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_INFO, message, args);
    va_end(args);
}

void a3LogFile::dev(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEV, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::dev(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEV, message, args);
    va_end(args);
}

void a3LogFile::debug(const char* prefix, const char* suffix, const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEBUG, prefix, suffix, message, args);
    va_end(args);
}

void a3LogFile::debug(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEBUG, message, args);
    va_end(args);
}
