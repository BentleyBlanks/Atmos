#include <core/a3Settings.h>
#include <stddef.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strstream>

// --！重大革新:小改动:BUG修复
a3Version atomsVersion = { 0, 0, 1 };

// 注意:只能在Debug模式下才能使用
void aa3Log(const char* string, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, string);
	vprintf(string, args);
	va_end(args);
#endif
}