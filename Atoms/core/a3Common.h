#include <core/a3Settings.h>
#include <assert.h>

template<class T>
std::string a3ToString(const std::vector<T>& values);

template <class T>
std::string a3ToString(const T& value);

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


