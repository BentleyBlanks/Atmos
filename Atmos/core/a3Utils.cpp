#include <core/a3Utils.h>
#include <core/log/a3Log.h>
#include <iostream>  
#include <sstream>
#include <iomanip>
#include <iterator>

// Copied from [OpenFrameworks](http://openframeworks.cc/)

std::string a3ToLower(const std::string & src)
{
    std::string dst = src;

    std::transform(dst.begin(), dst.end(), dst.begin(), ::tolower);

    return dst;
}

std::string a3ToUpper(const std::string & src)
{
    std::string dst = src;

    std::transform(dst.begin(), dst.end(), dst.begin(), ::toupper);

    return dst;
}

// 基础数值类型转为字符串
int a3ToInt(const std::string& intString)
{
    int x = 0;
    std::istringstream cur(intString);
    cur >> x;
    return x;
}

float a3ToFloat(const std::string& floatString)
{
    float x = 0;
    std::istringstream cur(floatString);
    cur >> x;
    return x;
}

double a3ToDouble(const std::string& doubleString)
{
    double x = 0;
    std::istringstream cur(doubleString);
    cur >> x;
    return x;
}

int64_t a3ToInt64(const std::string& intString)
{
    int64_t x = 0;
    std::istringstream cur(intString);
    cur >> x;
    return x;
}

bool a3ToBool(const std::string& boolString)
{
    auto lower = a3ToLower(boolString);
    if(lower == "true")
    {
        return true;
    }
    if(lower == "false")
    {
        return false;
    }
    bool x = false;
    std::istringstream cur(lower);
    cur >> x;
    return x;
}

char a3ToChar(const std::string& charString)
{
    char x = '\0';
    std::istringstream cur(charString);
    cur >> x;
    return x;
}

//template <>
//std::string a3ToHex(const std::string& value)
//{
//    std::ostringstream out;
//    // how many bytes are in the string
//    std::size_t numBytes = value.size();
//    for(std::size_t i = 0; i < numBytes; i++)
//    {
//        // print each byte as a 2-character wide hex value
//        out << std::setfill('0') << std::setw(2) << std::hex << (unsigned int) ((unsigned char) value[i]);
//    }
//    return out.str();
//}

std::string a3ToHex(const char* value)
{
    // this function is necessary if you want to print a string
    // using a syntax like ofToHex("test")
    return a3ToHex((std::string) value);
}

// 十六进制字符串转换
int a3HexToInt(const std::string& intHexString)
{
    int x = 0;
    std::istringstream cur(intHexString);
    cur >> std::hex >> x;
    return x;
}

char a3HexToChar(const std::string& charHexString)
{
    int x = 0;
    std::istringstream cur(charHexString);
    cur >> std::hex >> x;
    return (char) x;
}

float a3HexToFloat(const std::string& floatHexString)
{
    union intFloatUnion
    {
        //int x;
        uint32_t i;
        float f;
    } myUnion;
    myUnion.i = 0;
    std::istringstream cur(floatHexString);
    cur >> std::hex >> myUnion.i;
    return myUnion.f;
}

std::string a3HexToString(const std::string& stringHexString)
{
    std::stringstream out;
    std::stringstream stream(stringHexString);
    // a hex string has two characters per byte
    std::size_t numBytes = stringHexString.size() / 2;
    for(std::size_t i = 0; i < numBytes; i++)
    {
        std::string curByte;
        // grab two characters from the hex string
        stream >> std::setw(2) >> curByte;
        // prepare to parse the two characters
        std::stringstream curByteStream(curByte);
        int cur = 0;
        // parse the two characters as a hex-encoded int
        curByteStream >> std::hex >> cur;
        // add the int as a char to our output stream
        out << (char) cur;
    }
    return out.str();
}

//#include <assert.h>

//// --!Assert
//// 可用于编译期间字符串连接
//#define A3_CAT_AUX(a, b) a##b
//#define A3_CAT(a, b) A3_CAT_AUX(a, b)
//
//// 此为静态分析用宏 可自行实现
//#define ANALYSIS_ASSUME(x)
//
//// 当遇到这个断点时系统会很快的让我们选择是否调试。选中调试就可以容易的附加到进程
//#if defined (A3_PLATFORM_WINDOWS)
//// This produces an debug interrupt on 32-bit Intel/AMD processors
//#define A3DBGBREAK __debugbreak();
//
//#elif defined (A3_PLATFORM__PS3)
//#define A3DBGBREAK { __asm__ volatile("tw 31,0,0"); }
//
//#elif defined (A3_PLATFORM__WIIU)
//#define A3DBGBREAK OSDebug();
//
//#elif defined(A3_PLATFORM_PSP2)
//#include <libdbg.h>
//#define A3DBGBREAK SCE_BREAK();
//
//#elif defined (A3_PLATFORM_LINUX)
//#define A3DBGBREAK asm("int $3");
//
//#elif defined(A3_PLATFORM_IOS)
//#define A3DBGBREAK __builtin_trap();
//
//#elif defined(A3_PLATFORM_ANDROID)
//#define A3DBGBREAK raise(SIGTRAP);
//
//#elif defined(A3_PLATFORM_TIZEN)
//#include <signal.h>
//#define A3DBGBREAK raise(SIGTRAP);
//
//#elif defined(A3_PLATFORM_NACL)
//#ifdef _MSC_VER
//#define A3DBGBREAK __debugbreak();
//#else
//#define A3DBGBREAK __builtin_trap();
//#endif
//
//#else
////XBox 360, etc.
//#define A3DBGBREAK DebugBreak()
//#endif
//
//#ifdef _DEBUG
//#define A3ASSERT(x, ...)
//
//#else
//#define A3ASSERT(x)
//#endif
//
//#define A3ASSERT_ALWAYS_MSG(_exp, _msg, ...) \
//{ \
//    ANALYSIS_ASSUME(_exp) \
//    if(!(_exp)) \
//        if(A3ASSERT(A3_CAT(L, __FILE__), __LINE__, #_msg)) \
//            A3DBGBREAK; \
//}
//
//// 字符串前加L的解释 http://stackoverflow.com/questions/6384118/what-does-the-l-in-front-a-string-mean-in-c
//#define A3ASSERT_ALWAYS(x) \
//{ \
//    ANALYSIS_ASSUME(x) \
//    if(!(x)) \
//        if(A3ASSERT(A3_CAT(L, __FILE__), __LINE__, #x)) \
//            A3DBGBREAK; \
//}
//
//// 保证只触发一次的Assert 因此这样的断点也就可以忽略
//#define A3ASSERT_ONCE(_exp) \
//{ \
//    ANALYSIS_ASSUME(_exp) \
//    static bool s_once = true; \
//    if(s_once && !(_exp)) \
//    { \
//        A3ASSERT(_exp); \
//        s_once = false; \
//    } \
//}
//
//// 对于可变参宏__VA_ARGS__的分析可见 http://blog.csdn.net/yiya1989/article/details/7849588
//#define A3ASSERT_MSG(x, msg, ...) A3ASSERT_ALWAYS_MSG(x, msg, ##__VA_ARGS__)
//
//#define A3ASSERT_ONCE_MSG(_exp, _msg, ...) \
//{ \
//    ANALYSIS_ASSUME(_exp) \
//    static bool s_once = true; \
//    if(s_once && !(_exp)) \
//    { \
//        A3ASSERT_MSG(_exp, _msg, ##__VA_ARGS__); \
//        s_once = false; \
//    } \
//}