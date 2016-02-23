#ifndef ATOMS_SETTINGS_H
#define ATOMS_SETTINGS_H

// ƽ̨��
// �궨������:
// http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html
// OpenFrameworks->ofConstants.h�궨��
#if defined( __WIN32__ ) || defined( _WIN32 )
#define T2_PLATFORM_WINDOWS

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
#define A3_TOLERANCE_FLOAT 0.000001f
#define A3_TOLERANCE_DOUBLE 0.00000001


// Atoms�汾��Ϣ
// --!����box2d http://en.wikipedia.org/wiki/Software_versioning
struct a3Version
{
	int major;		///< significant changes
	int minor;		///< incremental changes
	int revision;		///< bug fixes

	void print()
	{
		std::cout << "Atoms�汾:" << major << "." << minor << "." << revision << std::endl;
	}
};

// ��ǰ�汾��
extern a3Version atomsVersion;

// Deprecated
// log�����box2d��b2Log
void aa3Log(const char* string, ...);


#endif