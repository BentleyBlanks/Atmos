#include <core/log/a3LogHTML.h>
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

#define CHECK_IFSTARTED \
    if(!a3LogHTML::isBegin) { \
        printf("System Error: Call a3LogHTML::log() without begin()\n"); \
        return; } 

bool a3LogHTML::isBegin = false;

std::ofstream* a3LogHTML::ofile = new std::ofstream();

//std::ifstream* a3LogHTML::ifile = new std::ifstream();

std::vector<std::string> logList;

void a3LogHTML::begin()
{
    isBegin = true;

    static char name[128];
    time_t cur;
    const struct tm* tm;

    cur = time(NULL);
    tm = localtime(&cur);

    sprintf(name, "%02d-%02d-%02d-Atoms-%s.html", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, atomsVersion.get().c_str());

    ofile->open(name);

    logList.push_back("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>Atoms Log</title></head><body>");

    logList.push_back("<font color=\"#000000\"> <br> === Atoms Log Start === <br><br><br><br> </font>");
}

void a3LogHTML::end()
{
    CHECK_IFSTARTED

    logList.push_back("<font color=\"#000000\"><br><br><br> === Atoms Log End === <br> </font></body></html>");
    
    for(auto l : logList)
        *ofile << l;

    ofile->close();
}

void a3LogHTML::log(a3LogLevel logLevel, const char* message, va_list args)
{
    const char* lv;
    time_t cur = time(NULL);
    const struct tm* tm = localtime(&cur);
    static char content[1024], timeBuffer[64], messageBuffer[1024], contentBuffer[1024];

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

    sprintf(timeBuffer, "[%02d:%02d:%02d]        [%s]        ", tm->tm_hour, tm->tm_min, tm->tm_sec, lv);
    // messageBuffer:[hour]:[minute]:[second]
    strcpy(messageBuffer, timeBuffer);
    // messageBuffer:[hour]:[minute]:[second] message ...
    strcat(messageBuffer, message);
    // messageBuffer:[hour]:[minute]:[second] message'
    vsprintf(contentBuffer, messageBuffer, args);

    std::string msg(contentBuffer);

    std::size_t found = msg.find("&");
    if(found != std::string::npos)
        msg.replace(found, 1, "$amp");

    found = msg.find("<");
    if(found != std::string::npos)
        msg.replace(found, 1, "&lt");

    found = msg.find(">");
    if(found != std::string::npos)
        msg.replace(found, 1, "&gt");

    found = msg.find("\n");
    if(found != std::string::npos)
        msg.replace(found, 1, "<br>");

    switch(logLevel)
    {
    case a3LogLevel::A3_LOG_LEVEL_BEGINBLOCK:
        sprintf(content, "<br><font color=\"#8080FF\"><b> <u>%s</u> </b> (%s) </font><table width=100%% border=0><tr width=100%%><td width=10></td><td width=*>\n", msg.c_str(), "tag");
        break;
    case a3LogLevel::A3_LOG_LEVEL_ENDBLOCK:
        sprintf(content, "</td></tr></table><font color=\"#8080FF\"><b> %s </b></font>\n", "");
        break;
    case a3LogLevel::A3_LOG_LEVEL_FATALERROR:
        sprintf(content, "<font color=\"#FF0000\"><b>%s</b></font><br>\n", msg.c_str());
        break;
    case a3LogLevel::A3_LOG_LEVEL_ERROR:
        sprintf(content, "<font color=\"#FF0000\"><b>%s</b></font><br>\n", msg.c_str());
        break;
    case a3LogLevel::A3_LOG_LEVEL_SERIOUSWARNING:
        sprintf(content, "<font color=\"#FF4000\"><b>%s</b></font><br>\n", msg.c_str());
        break;
    case a3LogLevel::A3_LOG_LEVEL_WARNING:
        sprintf(content, "<font color=\"#FF8000\">%s</font><br>\n", msg.c_str());
        break;
    case a3LogLevel::A3_LOG_LEVEL_SUCCESS:
        sprintf(content, "<font color=\"#009000\">%s</font><br>\n", msg.c_str());
        break;
    case a3LogLevel::A3_LOG_LEVEL_INFO:
        sprintf(content, "<font color=\"#000000\">%s</font><br>\n", msg.c_str());
        break;
    case a3LogLevel::A3_LOG_LEVEL_DEV:
        sprintf(content, "<font color=\"#3030F0\">%s</font><br>\n", msg.c_str());
        break;
    case a3LogLevel::A3_LOG_LEVEL_DEBUG:
        sprintf(content, "<font color=\"#00FFFF\">%s</font><br>\n", msg.c_str());
        break;
    default:
        return;
    }

    logList.push_back(std::string(content));
}

void a3LogHTML::log(a3LogLevel logLevel, const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(logLevel, message, args);
    va_end(args);
}

void a3LogHTML::fatalError(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_FATALERROR, message, args);
    va_end(args);
}

void a3LogHTML::error(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_ERROR, message, args);
    va_end(args);
}

void a3LogHTML::seriousWarning(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SERIOUSWARNING, message, args);
    va_end(args);
}

void a3LogHTML::warning(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_WARNING, message, args);
    va_end(args);
}

void a3LogHTML::success(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_SUCCESS, message, args);
    va_end(args);
}

void a3LogHTML::info(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_INFO, message, args);
    va_end(args);
}

void a3LogHTML::dev(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEV, message, args);
    va_end(args);
}

void a3LogHTML::debug(const char* message, ...)
{
    CHECK_IFSTARTED

    va_list args;
    va_start(args, message);
    log(A3_LOG_LEVEL_DEBUG, message, args);
    va_end(args);
}









