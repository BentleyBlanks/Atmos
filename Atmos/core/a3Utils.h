#pragma once

#include <core/a3Settings.h>
#include <core/log/a3Log.h>

// wstring string convert
std::wstring a3S2WS(const std::string& str);

std::string a3WS2S(const std::wstring& wstr);

// Copied from [OpenFrameworks](http://openframeworks.cc/)
// Converts all characters in a string to lowercase.
std::string a3ToLower(const std::string& src);

// Converts all characters in the string to uppercase.
std::string a3ToUpper(const std::string& src);

// 基础数值类型转为字符串
// Convert a value to a string.
// a3ToString does its best to convert any value to a string. If the data type
// implements a stream << operator, then it will be converted.
template <class T>
std::string a3ToString(const T& value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

// Convert a value to a string with a specific precision.
// Like sprintf "%4f" format, in this example precision=4
template <class T>
std::string a3ToString(const T& value, int precision)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

// Convert a value to a string with a specific width and fill
// Like sprintf "% 4d" or "% 4f" format, in this example width=4, fill=' '
template <class T>
std::string a3ToString(const T& value, int width, char fill)
{
    std::ostringstream out;
    out << std::fixed << std::setfill(fill) << std::setw(width) << value;
    return out.str();
}

// Convert a value to a string with a specific precision, width and filll
// Like sprintf "%04.2d" or "%04.2f" format, in this example precision=2, width=4, fill='0'
template <class T>
std::string a3ToString(const T& value, int precision, int width, char fill)
{
    std::ostringstream out;
    out << std::fixed << std::setfill(fill) << std::setw(width) << std::setprecision(precision) << value;
    return out.str();
}

// Convert a vector of values to a comma-delimited string.
// This method will take any vector of values and output a list of the values
// as a comma-delimited string.
template<class T>
std::string a3ToString(const std::vector<T>& values)
{
    std::stringstream out;
    int n = values.size();
    out << "{";
    if(n > 0)
    {
        for(int i = 0; i < n - 1; i++)
        {
            out << values[i] << ", ";
        }
        out << values[n - 1];
    }
    out << "}";
    return out.str();
}

// 字符串转为其他基础类型
// Convert a string to an integer.
int a3ToInt(const std::string& intString);

// Convert a string to a int64_t.
int64_t a3ToInt64(const std::string& intString);

// Convert a string to a float.
float a3ToFloat(const std::string& floatString);

// Convert a string to a double.
double a3ToDouble(const std::string& doubleString);

// Convert a string to a boolean.
bool a3ToBool(const std::string& boolString);

// Convert a string representation of a char to a actual char.
char a3ToChar(const std::string& charString);

// Converts any value to its equivalent hexadecimal representation.
// The hexadecimal representation corresponds to the way a number is stored in
// memory.
template <class T>
std::string a3ToHex(const T& value)
{
    std::ostringstream out;
    // pretend that the value is a bunch of bytes
    unsigned char* valuePtr = (unsigned char*) &value;
    // the number of bytes is determined by the datatype
    int numBytes = sizeof(T);
    // the bytes are stored backwards (least significant first)
    for(int i = numBytes - 1; i >= 0; i--)
    {
        // print each byte out as a 2-character wide hex value
        out << std::setfill('0') << std::setw(2) << std::hex << (int) valuePtr[i];
    }
    return out.str();
}

// Convert a string to a hexadecimal string.
template <class T>
std::string a3ToHex(const std::string& value)
{
    std::ostringstream out;
    // pretend that the value is a bunch of bytes
    unsigned char* valuePtr = (unsigned char*) &value;
    // the number of bytes is determined by the datatype
    int numBytes = sizeof(T);
    // the bytes are stored backwards (least significant first)
    for(int i = numBytes - 1; i >= 0; i--)
    {
        // print each byte out as a 2-character wide hex value
        out << std::setfill('0') << std::setw(2) << std::hex << (int) valuePtr[i];
    }
    return out.str();
}

// Convert a c-style string to a hexadecimal string.
std::string a3ToHex(const char* value);

// Convert a string representing an integer in hexadecimal to a string.
int a3HexToInt(const std::string& intHexString);

// Convert a string representing an char in hexadecimal to a char.
char a3HexToChar(const std::string& charHexString);

// Convert a string representing an float in hexadecimal to a float.
float a3HexToFloat(const std::string& floatHexString);

// Convert a string representing an string in hexadecimal to a string.
std::string a3HexToString(const std::string& stringHexString);

// Log
// reporting that the given function is not implemented
#define a3FuncNotImplementedError()\
{\
    std::string error = "Unimplemented ";\
    error += __FUNCTION__;\
    error += "() method called";\
    a3Log::error(error.c_str());\
}

// reporting a warning that ptr is nullptr
#define a3NullPtrWarning(ptrName)\
{\
    std::string error;\
    error += __FUNCTION__;\
    error += "() given ";\
    error += ptrName;\
    error += "is null";\
    a3Log::warning(error.c_str());\
}