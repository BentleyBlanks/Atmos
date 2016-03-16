#include <core/a3Settings.h>
#include <stddef.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strstream>
#include <sstream>
#include <core/a3Common.h>

// ���ؿɼ���Χ��ȫ�ֱ���
a3Version atomsVersion;

// ע��:ֻ����Debugģʽ�²���ʹ��
void t2Log(const char* string, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, string);
	vprintf(string, args);
	va_end(args);
#endif
}

std::string a3Version::get()
{
    std::ostringstream out;
    out << type << major << minor << revision;
    return out.str();
}

void a3Version::print()
{
    std::cout << "Atoms�汾:" << type << " " << major << "." << minor << "." << revision << std::endl;
}

// --���ش����:С�Ķ�:BUG�޸�
a3Version::a3Version() : major(0), minor(0), revision(2), type("Alpha")
{

}
