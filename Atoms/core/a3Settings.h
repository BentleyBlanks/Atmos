#ifndef ATOMS_SETTINGS_H
#define ATOMS_SETTINGS_H

// 平台宏
// 宏定义借鉴于:
// http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html
// OpenFrameworks->ofConstants.h宏定义
#if defined( __WIN32__ ) || defined( _WIN32 )
#define T2_PLATFORM_WINDOWS

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
// 实现cout << string 不包含则报错
#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
// 包含min&max函数
#include <algorithm>
#include <assert.h>
#include <math.h>

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

// 浮点数判断容差
#define A3_TOLERANCE_INT 0
#define A3_TOLERANCE_FLOAT 0.000001f
#define A3_TOLERANCE_DOUBLE 0.00000001


// Atoms版本信息
// --!来自box2d http://en.wikipedia.org/wiki/Software_versioning
struct a3Version
{
	int major;		///< significant changes
	int minor;		///< incremental changes
	int revision;		///< bug fixes

	void print()
	{
		std::cout << "Atoms版本:" << major << "." << minor << "." << revision << std::endl;
	}
};

// 当前版本号
extern a3Version atomsVersion;

// Deprecated
// log借鉴于box2d的b2Log
void aa3Log(const char* string, ...);


#endif