#ifndef T3_VECTOR2_INL
#define T3_VECTOR2_INL

#include <t3Math/core/t3Vector2.h>
#include <string>

template <class Type>
inline t3Vector2<Type>::t3Vector2() :x(0), y(0) {}

template <class Type>
inline t3Vector2<Type>::t3Vector2(Type x, Type y) : x(x), y(y) {}

// 自类型拷贝构造
template <class Type>
inline t3Vector2<Type>::t3Vector2(const t3Vector2<Type>& vector) : x(vector.x), y(vector.y) {}

// t3Vector2<Type>->t3Vector2<Type2> explict防止隐式转换
template<typename Type>
template <typename Type2>
inline t3Vector2<Type>::t3Vector2(const t3Vector2<Type2> &vector) : x(vector.x), y(vector.y) {}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::clone(const t3Vector2<Type>& vector)
{
    x = vector.x; y = vector.y;
}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::set(t3Vector2& vector)
{
    set(vector.x, vector.y);
}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::set(const Type x, const Type y)
{
    this->x = x; this->y = y;
}

// 置为零向量
template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::zero()
{
    // --!之所以此处不(Type)强制类型转换 是因为意义不大 但是却消耗比较大的速度
    x = 0;
    y = 0;
}

// 将此2D向量标准化
template <class Type>
T3_FORCE_INLINE bool t3Vector2<Type>::normalize()
{
    Type length = getLength();
    if(length == 0)
        return false;

    Type inverseLength = 1.0f / length;
    x *= inverseLength;
    y *= inverseLength;

    return true;
}

// 求模长
template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getLength() const
{
    return sqrtf(x*x + y*y);
}

template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getLengthSquare() const
{
    return (x*x + y*y);
}

// 叉积
template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::cross(const t3Vector2<Type>& vector) const
{
    // |i j k|
    // |a b c|
    // |x y z|
    // |axb|的长度代表了以a和b为邻边的平行四边形的面积
    return x*vector.y - y*vector.x;
}

// 与一标量叉积的结果为 先求得当前向量的垂直向量(-y, x)再缩放scalar倍
// 叉积
template <class Type>
T3_FORCE_INLINE t3Vector2<Type> t3Vector2<Type>::cross(const float& scalar) const
{
    return t3Vector2<Type>(-this->y*scalar, scalar*this->x);
}

template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::dot(const t3Vector2<Type>& vector) const
{
    return this->x*vector.x + this->y*vector.y;
}

// 两点间距离
template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getDistance(const t3Vector2<Type>& end) const
{
    return sqrt((end.x - x)*(end.x - x) + (end.y - y)*(end.y - y));
}

template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getDistanceSquare(const t3Vector2<Type>& end) const
{
    return (end.x - x)*(end.x - x) + (end.y - y)*(end.y - y);
}

// 测试使用的Console Log输出
template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::print(std::string name)
{
#ifdef _DEBUG
    // 为空
    if(name.empty())
        name = "";
    std::cout << "t3Vector2 ";
    std::cout << name << "(x: " << x << ", y: " << y << ")" << std::endl;
#endif
}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::print()
{
#ifdef _DEBUG
    std::cout << "t3Vector2 ";
    std::cout << "(x: " << x << ", y: " << y << ")" << std::endl;
#endif
}

// 成员函数
// 重载 =
template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator=(const t3Vector2<Type>& vector)
{
    this->x = vector.x;
    this->y = vector.y;

    return *this;
}

// 重载 == !=
template <class Type>
inline bool t3Vector2<Type>::operator==(const t3Vector2<Type>& vector) const
{
    return equals(this->x, vector.x) && equals(this->y, vector.y);
}

template <class Type>
inline bool t3Vector2<Type>::operator!=(const t3Vector2<Type>& vector) const
{
    return !(equals(this->x, vector.x) && equals(this->y, vector.y));
}

// 重载一元 - +
template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator-()
{
    t3Vector2<Type> v;
    v.set(-x, -y);
    return v;
}

template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator+()
{
    return *this;
}

// 重载 二元 + -
template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator+(const t3Vector2<Type>& vector) const
{
    //--!加减号和左右乘顺序相同
    return t3Vector2<Type>(x + vector.x, y + vector.y);
}

template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator-(const t3Vector2<Type>& vector) const
{
    return t3Vector2<Type>(x - vector.x, y - vector.y);
}

// 标量 乘除法 * / scale
template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator*(const Type scale) const
{
    return t3Vector2<Type>(x * scale, y * scale);
}

template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator/(const Type scale) const
{
    if(scale == 0)
    {
#ifdef _DEBUG
        // --!未来替换为log
        std::cout << "除数不能为零!" << std::endl;
#endif
    }
    return t3Vector2<Type>(x / scale, y / scale);
}

// 重载 += -= *= /=
template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator+=(const t3Vector2<Type>& vector)
{
    //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
    x += vector.x;
    y += vector.y;
    return *this;
}

template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator-=(const t3Vector2<Type>& vector)
{
    //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
    x -= vector.x;
    y -= vector.y;
    return *this;
}

template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator*=(const Type scale)
{
    //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
    x *= scale;
    y *= scale;
    return *this;
}

template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator/=(const Type scale)
{
    //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
    x /= scale;
    y /= scale;
    return *this;
}

// 向量 * vector(无需T3_FORCE_INLINE一个右乘 因为类型相同左右不分)
template <class Type>
inline Type t3Vector2<Type>::operator*(const t3Vector2<Type>& vector) const
{
    return (x*vector.x + y*vector.y);
}


// 非成员函数
// 左乘标量
// --!float类型的Scale是为了避免Scale模板类型与Martix类型冲突导致编译器无法找到合理类型
// template<class Scale, class Type>
template<class Type>
inline t3Vector2<Type> operator*(const float scale, const t3Vector2<Type>& vector)
{
    return t3Vector2<Type>(scale * vector.x, scale * vector.y);
}


// vector左叉积一标量
// 实质为将vector左旋90度得到tempV 之后再缩放scalar倍
template<class Type>
T3_FORCE_INLINE t3Vector2<Type> t2Cross(const Type scalar, const t3Vector2<Type>& vector)
{
    return t3Vector2<Type>(-scalar*vector.y, scalar*vector.x);
}

#endif