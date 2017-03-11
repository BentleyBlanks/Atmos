#ifndef A3_LOGHTML_H
#define A3_LOGHTML_H

#include <core/log/a3Log.h>

// not support UTF8 log to file
// default file name format [year]-[month]-[day]-Atmos-VertionType.html
// would cover the same name file, be sure to end() when everything is finished
class a3LogHTML
{
public:
    static void log(a3LogLevel logLevel, const char* message, ...);

    static void begin();

    static void end();

    // log to default name file: [prefix]:Atmos-VerisionType.html
    static void fatalError(const char* message, ...);

    static void error(const char* message, ...);

    static void seriousWarning(const char* message, ...);

    static void warning(const char* message, ...);

    static void success(const char* message, ...);

    static void info(const char* message, ...);

    static void dev(const char* message, ...);

    static void debug(const char* message, ...);

private:
    a3LogHTML() {}
    a3LogHTML(const a3LogHTML&) {}
    ~a3LogHTML() {}
    a3LogHTML& operator=(const a3LogHTML& event) const {}

    static void log(a3LogLevel logLevel, const char* message, va_list args);

    static std::ofstream* ofile;

    static bool isBegin;
};


#endif