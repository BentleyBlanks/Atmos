#include <core/a3Settings.h>
#include <stddef.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strstream>

// --���ش����:С�Ķ�:BUG�޸�
a3Version atomsVersion = { 0, 0, 1 };

// ע��:ֻ����Debugģʽ�²���ʹ��
void aa3Log(const char* string, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, string);
	vprintf(string, args);
	va_end(args);
#endif
}