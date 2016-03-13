#ifndef A3_COMMON_INL
#define A3_COMMON_INL

#include <core/a3Common.h>
#include <sstream>

template<class T>
std::string a3ToString(const std::vector<T>& values)
{
    stringstream out;
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

template <class T>
std::string a3ToString(const T& value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

#endif