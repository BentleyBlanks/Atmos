#ifndef A3_LOGFILE_H
#define A3_LOGFILE_H

#include <core/log/a3Log.h>

// not support UTF8 log to file
// file name format [year]-[month]-[day]-[prefix].[suffix]
// file content format [hour]:[minute]:[second] [log level] [message]  [...]
class a3LogFile
{
public:
    static void log(a3LogLevel logLevel, const char* prefix, const char* suffix, const char* message, ...);

    static void log(a3LogLevel logLevel, const char* message, ...);

    // log to default name file: [prefix]:Atmos-VerisionType [suffix]:log
    static void fatalError(const char* message, ...);

    static void error(const char* message, ...);

    static void seriousWarning(const char* message, ...);

    static void warning(const char* message, ...);

    static void success(const char* message, ...);

    static void info(const char* message, ...);

    static void dev(const char* message, ...);

    static void debug(const char* message, ...);

    // log to custom name log file
    static void fatalError(const char* prefix, const char* suffix, const char* message ...);

    static void error(const char* prefix, const char* suffix, const char* message ...);

    static void seriousWarning(const char* prefix, const char* suffix, const char* message ...);

    static void warning(const char* prefix, const char* suffix, const char* message ...);

    static void success(const char* prefix, const char* suffix, const char* message ...);

    static void info(const char* prefix, const char* suffix, const char* message ...);

    static void dev(const char* prefix, const char* suffix, const char* message ...);

    static void debug(const char* prefix, const char* suffix, const char* message ...);

private:
    a3LogFile() {}
    a3LogFile(const a3LogFile&) {}
    ~a3LogFile() {}
    a3LogFile& operator=(const a3LogFile& event) const {}

    static void log(a3LogLevel logLevel, const char* prefix, const char* suffix, const char* message, va_list args);

    static void log(a3LogLevel logLevel, const char* message, va_list args);

    static std::ofstream* ofile;
    static std::ifstream* ifile;
};

#endif