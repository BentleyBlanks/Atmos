#ifndef ATOMS_SETTINGS_H
#define ATOMS_SETTINGS_H

// ------------------------Platforms------------------------
// �궨������:
// http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html
// OpenFrameworks->ofConstants.h�궨��
#if defined( __WIN32__ ) || defined( _WIN32 )
#define A3_PLATFORM_WINDOWS

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN
// --!����ģ���е�max��Visual C++�е�ȫ�ֵĺ�max��ͻ
// --!http://stackoverflow.com/questions/1904635/warning-c4003-and-errors-c2589-and-c2059-on-x-stdnumeric-limitsintmax

// Appleƽ̨
#elif defined( __APPLE_CC__)
#include <TargetConditionals.h>

// iOS֧�� ��û��iPad����
#if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
#define A3_PLATFORM_IPHONE
#define A3_PLATFORM_IOS
#define A3_PLATFORM_OPENGLES

// Macƽ̨��֧��
#else
#define A3_PLATFORM_OSX
#endif

// Androidƽ̨��֧��
#elif defined (__ANDROID__)
#define A3_PLATFORM_ANDROID
#define A3_PLATFORM_OPENGLES

// Linuxƽ̨��֧��
#elif defined(__ARMEL__)
#define A3_PLATFORM_LINUX
#define A3_PLATFORM_OPENGLES
#define A3_PLATFORM_LINUX_ARM
#else
#define A3_PLATFORM_LINUX
#endif

#include <iostream>
// ʵ��cout << string �������򱨴�
#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
// ����min&max����
#include <algorithm>
#include <assert.h>
#include <math.h>

// ------------------------Assert------------------------
#ifndef A3ASSERT
#define A3ASSERT(exp, errorMessage) assert(exp && errorMessage)
#endif


// ------------------------Deprecated------------------------
// ���ú�ֻ��Windowsƽ̨��ʹ��
// ��ȻҲ����ѡ��Deprecated��ȫ�ֹر�
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

// ------------------------Math Constants------------------------
#ifndef PI
#define PI 3.1415926535897
#endif

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

// �������ж��ݲ�
#define A3_TOLERANCE_INT 0
#define A3_TOLERANCE_FLOAT 1e-6
#define A3_TOLERANCE_DOUBLE 1e-8
// --!��� https://www.zhihu.com/question/41895326
// Atomsȫ���ཻ����ģ��Mitsuba����double����
#define A3_RAY_BIAS 1e-4



// ------------------------Image------------------------
//#define A3_IMAGE_LIB_PNGPP
#define A3_IMAGE_LIB_LODEPNG

enum a3ImageType
{
    A3_IMAGE_ERROR = -1,
    A3_IMAGE_PPM = 0,
    A3_IMAGE_PNG = 1
};

// ------------------------Scene------------------------
#define A3_SCENE_WORLDRADIUS 100000

// ------------------------Version------------------------
// Atoms�汾��Ϣ
// --!����box2d http://en.wikipedia.org/wiki/Software_versioning
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

// ��ǰ�汾��
extern a3Version atomsVersion;

// Deprecated
// log�����box2d��b2Log
A3_DEPRECATED void t2Log(const char* string, ...);

#endif