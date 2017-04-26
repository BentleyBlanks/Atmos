#ifndef ATMOS_SETTINGS_H
#define ATMOS_SETTINGS_H

// ------------------------Platforms------------------------
// 宏定义借鉴于:
// http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html
// OpenFrameworks->ofConstants.h宏定义
#if defined( __WIN32__ ) || defined( _WIN32 )
#define A3_PLATFORM_WINDOWS

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN
// --!函数模板中的max与Visual C++中的全局的宏max冲突
// --!http://stackoverflow.com/questions/1904635/warning-c4003-and-errors-c2589-and-c2059-on-x-stdnumeric-limitsintmax

// Apple平台
#elif defined( __APPLE_CC__)
#include <TargetConditionals.h>

// iOS支持 并没有iPad出现
#if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
#define A3_PLATFORM_IPHONE
#define A3_PLATFORM_IOS
#define A3_PLATFORM_OPENGLES

// Mac平台的支持
#else
#define A3_PLATFORM_OSX
#endif

// Android平台的支持
#elif defined (__ANDROID__)
#define A3_PLATFORM_ANDROID
#define A3_PLATFORM_OPENGLES

// Linux平台的支持
#elif defined(__ARMEL__)
#define A3_PLATFORM_LINUX
#define A3_PLATFORM_OPENGLES
#define A3_PLATFORM_LINUX_ARM
#else
#define A3_PLATFORM_LINUX
#endif

#include <iostream>
#include <sstream>
// 实现cout << string 不包含则报错
#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
// 包含min&max函数
#include <algorithm>
#include <assert.h>
#include <math.h>

//#define A3_SAFE_DELETE(pointer) if(pointer)\
//{\
//    delete pointer;\
//    pointer = NULL;\
//}

template<typename T> 
void A3_SAFE_DELETE(T*& a)
{
    if(a)
    {
        delete a;
        a = NULL;
    }
}

#define A3_SAFE_DELETE_1DARRAY(pointer) if(pointer)\
{\
    delete[] pointer;\
    pointer = NULL;\
}

// ------------------------Assert------------------------
#ifndef A3ASSERT
#define A3ASSERT(exp, errorMessage) assert(exp && errorMessage)
#endif


// ------------------------Deprecated------------------------
// 弃用宏只在Windows平台上使用
// 当然也可以选择将Deprecated宏全局关闭
#ifdef A3_IGNORE_DEPRECATION_GLOBALLY

#define A3_DEPRECATED
#define A3_DEPRECATED
#define A3_DISABLE_DEPRECATION
#define A3_ENABLE_DEPRECATION

#endif

// GCC
#if defined(__GCC__) || defined(__SNC__) || defined(__GNUC__)
#define A3_DEPRECATED __attribute__ ((deprecated))

///<MSVC 7.0
#elif defined(_MSC_VER) && (_MSC_VER >= 1300)
#define  A3_DEPRECATED __declspec(deprecated)

#else
///< CWCC doesn't seem to have such a 'marker' facility.
#define A3_DEPRECATED 
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1300) ///<MSVC 7.0
// On Windows this macro will mark code as deprecated.
#define A3_DEPRECATED __declspec(deprecated)

// This macro will temporarily disable the deprecated warning, allowing you to use deprecated code without getting a warning.
#define A3_DISABLE_DEPRECATION \
    __pragma (warning(push)) \
    __pragma (warning(disable : 4996))

// This macro will enable the deprecated warning again, this should be used at the end of a code block which had this warning disabled.
#define A3_ENABLE_DEPRECATION \
    __pragma (warning(pop))
#else
// On Non-Windows platforms this macro is ignored.
#define A3_DEPRECATED
// On Non-Windows platforms this macro is ignored.
#define A3_DISABLE_DEPRECATION
// On Non-Windows platforms this macro is ignored.
#define A3_ENABLE_DEPRECATION
#endif

// 断言
#ifdef NDEBUG
#define a3Assert(expr) ((void)0)
#else
#define a3Assert(expr) \
    ((expr) ? (void)0 : \
        printf("Assertion \"%s\" failed in %s, line %d", \
               #expr, __FILE__, __LINE__))
#endif // NDEBUG

// ------------------------Math Constants------------------------
#ifndef PI
#define PI 3.1415926535897
#endif

#define A3_SQRT2 1.4142135623730950488016887242097
#define A3_SQRT3 1.7320508075688772935274463415059

#define A3_EXIT(value) std::exit(value);

#ifndef A3_INFINITY
#define A3_INFINITY FLT_MAX
#endif

// math
#ifndef A3_DEGREE_TO_RADIAN
#define A3_DEGREE_TO_RADIAN (PI/180.0f)
#endif

#ifndef A3_RADIAN_TO_DEGREE
#define A3_RADIAN_TO_DEGREE (180.0f/PI)
#endif

// 浮点数判断容差
#define A3_TOLERANCE_INT 0
#define A3_TOLERANCE_FLOAT 1e-6f
#define A3_TOLERANCE_DOUBLE 1e-8

// --!详见 https://www.zhihu.com/question/41895326
// Atmos全局相交测试模仿Mitsuba采用double精度
#define A3_RAY_BIAS 1e-3f

// 默认最大跟踪路径深度
#define A3_DEFAULT_MAX_DEPTH -1

// 没有实现重要性采样之前设定的概率密度通配符
#define A3_PDF_NOTNULL 0.5f

// 渲染器网格渲染划分力度
#define A3_GRID_LEVELX 5
#define A3_GRID_LEVELY 5

// ------------------------Image------------------------
#define A3_IMAGE_CHECKERBOARD_LEVEL 22

//#define A3_IMAGE_LIB_PNGPP
#define A3_IMAGE_LIB_LODEPNG
#define A3_IMAGE_LIB_TINYEXR

enum a3ImageType
{
    A3_IMAGE_ERROR = -1,
    A3_IMAGE_PPM = 0,
    A3_IMAGE_PNG = 1,
    A3_IMAGE_EXR = 2
};

// ------------------------Scene------------------------
#define A3_SCENE_WORLDRADIUS 100000

// Atmos支持三角形背面剔除，这可能会在画面上带来巨大变化
//#define A3_BACKFACE_CULLING

// ------------------------Version------------------------
// Atmos版本信息
// --!来自box2d http://en.wikipedia.org/wiki/Software_versioning
struct a3Version
{
    a3Version();

    std::string get();

    void print();	
    
    int major;		// significant changes

    int minor;		// incremental changes

    int revision;		// bug fixes

    std::string type;   // build type
};

// 当前版本号
extern a3Version atmosVersion;

// Deprecated
// log借鉴于box2d的b2Log
A3_DEPRECATED void t2Log(const char* string, ...);

#endif