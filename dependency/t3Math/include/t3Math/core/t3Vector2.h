#ifndef T3_VECTOR2_H
#define T3_VECTOR2_H

#include <iostream>
#include <string>
#include <t3Math/core/t3MathSettings.h>

template <class Type>
class t3Vector2
{
public:
	Type x, y;

    t3Vector2();

    t3Vector2(Type x, Type y);

	// 复制构造
    t3Vector2(const t3Vector2<Type>& vector);

    // t3Vector2<Type>->t3Vector2<Type2>
    template <typename Type2>
    explicit t3Vector2(const t3Vector2<Type2> &p);

    T3_FORCE_INLINE void clone(const t3Vector2<Type>& vector);

    T3_FORCE_INLINE void set(t3Vector2& vector);

    T3_FORCE_INLINE void set(const Type x, const Type y);

	// 重载 =
	inline t3Vector2<Type>& operator=(const t3Vector2<Type>& vector);
	// 重载 == !=
    inline bool operator==(const t3Vector2<Type>& vector) const;
    inline bool operator!=(const t3Vector2<Type>& vector) const;

	// 重载一元 - +
    inline t3Vector2<Type> operator-();
    inline t3Vector2<Type> operator+();

	// 重载 二元 + -
    inline t3Vector2<Type> operator+(const t3Vector2<Type>& vector) const;
    inline t3Vector2<Type> operator-(const t3Vector2<Type>& vector) const;

	// 标量 乘除法 * / scale
    inline t3Vector2<Type> operator*(const Type scale) const;
	// 会对
    inline t3Vector2<Type> operator/(const Type scale) const;

	// 重载 += -= *= /=
    inline t3Vector2<Type>& operator+=(const t3Vector2<Type>& vector);
    inline t3Vector2<Type>& operator-=(const t3Vector2<Type>& vector);
    inline t3Vector2<Type>& operator*=(const Type scale);
    inline t3Vector2<Type>& operator/=(const Type scale);

	// 向量 * vector(无需inline一个右乘 因为类型相同左右不分)
    inline Type operator*(const t3Vector2<Type>& vector) const;

	// 置为零向量
    T3_FORCE_INLINE void zero();

	// 将此2D向量标准化
    T3_FORCE_INLINE bool normalize();

	// 求模长
    T3_FORCE_INLINE Type getLength() const;

    T3_FORCE_INLINE Type getLengthSquare() const;

	// 叉积
    T3_FORCE_INLINE Type cross(const t3Vector2<Type>& vector) const;

	// 与一标量叉积的结果为 先求得当前向量的垂直向量(-y, x)再缩放scalar倍
	// 叉积
    T3_FORCE_INLINE t3Vector2<Type> cross(const float& scalar) const;

    T3_FORCE_INLINE Type dot(const t3Vector2<Type>& vector) const;

	// 两点间距离
    T3_FORCE_INLINE Type getDistance(const t3Vector2<Type>& end) const;

    T3_FORCE_INLINE Type getDistanceSquare(const t3Vector2<Type>& end) const;

	// 测试使用的Console Log输出
    T3_FORCE_INLINE void print(std::string name);

    T3_FORCE_INLINE void print();

};

#include <t3Math/core/t3Vector2.inl>

// 特化
typedef t3Vector2<double> t3Vector2d;
typedef t3Vector2<float> t3Vector2f;
typedef t3Vector2<int> t3Vector2i;

// t2Point
typedef t3Vector2d t2Point2d;
typedef t3Vector2f t2Point2f;
typedef t3Vector2i t2Point2i;

//// 零向量
//static t3Vector2f t3Vector2fZero;
//static t3Vector2i t3Vector2iZero;


#endif