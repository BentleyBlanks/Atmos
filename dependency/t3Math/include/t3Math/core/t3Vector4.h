#ifndef T3_VECTOR4_H
#define T3_VECTOR4_H

#include <t3Math/core/t3MathSettings.h>
#include <string>

template <class Type>
class t3Vector4
{
public:
    static const int DIM = 4;

    Type x;
    Type y;
    Type z;
    Type w;

    t3Vector4();

    // assigns scalar to x, y, and z
    explicit t3Vector4(Type _scalar);

    t3Vector4(Type _x, Type _y, Type _z, Type _w);

    t3Vector4(const t3Vector4& vec);

    T3_FORCE_INLINE Type * getPtr(){ return (Type*) &x;}

    T3_FORCE_INLINE const Type * getPtr() const{ return (const Type *) &x; }

    inline Type& operator[](int n){ return getPtr()[n]; }

    inline Type operator[](int n) const{ return getPtr()[n]; }

    // Getters and Setters.
    T3_FORCE_INLINE void set(Type _scalar);
    T3_FORCE_INLINE void set(Type _x, Type _y, Type _z, Type _w);
    T3_FORCE_INLINE void set(const t3Vector4<Type>& vec);

    // Check similarity/equality.
    inline bool operator==(const t3Vector4<Type>& vector) const;
    inline bool operator!=(const t3Vector4<Type>& vector) const;
    T3_FORCE_INLINE bool match(const t3Vector4<Type>& vector, float tolerance = 0.0001f) const;

    inline t3Vector4<Type>  operator+(const t3Vector4<Type>& vector) const;
    inline t3Vector4<Type>& operator+=(const t3Vector4<Type>& vector);
    inline t3Vector4<Type>& operator-=(const t3Vector4<Type>& vector);
    inline t3Vector4<Type>  operator-(const t3Vector4<Type>& vector) const;

    inline t3Vector4<Type>  operator-() const;
    inline t3Vector4<Type>  operator/(const t3Vector4<Type>& vector) const;
    inline t3Vector4<Type>& operator/=(const t3Vector4<Type>& vector);
    inline t3Vector4<Type> operator*(const t3Vector4<Type>& vector) const;
    inline t3Vector4<Type>& operator*=(const t3Vector4<Type>& vector);

    inline t3Vector4<Type>  operator+(const Type scalar) const;
    inline t3Vector4<Type>& operator+=(const Type scalar);
    inline t3Vector4<Type>  operator-(const Type scalar) const;
    inline t3Vector4<Type>& operator-=(const Type scalar);
    inline t3Vector4<Type>  operator*(const Type scalar) const;
    inline t3Vector4<Type>& operator*=(const Type scalar);
    inline t3Vector4<Type>  operator/(const Type scalar) const;
    inline t3Vector4<Type>& operator/=(const Type scalar);

    //friend ostream& operator<<(ostream& os, const t3Vector4<Type>& vec);
    //friend istream& operator>>(istream& is, const t3Vector4<Type>& vec);

    T3_FORCE_INLINE t3Vector4<Type> getScaled(const float length) const;

    T3_FORCE_INLINE t3Vector4<Type>& scale(const float length);

    T3_FORCE_INLINE float distance(const t3Vector4<Type>& pnt) const;

    T3_FORCE_INLINE float squareDistance(const t3Vector4<Type>& pnt) const;

    T3_FORCE_INLINE t3Vector4<Type> getInterpolated(const t3Vector4<Type>& pnt, float p) const;

    T3_FORCE_INLINE t3Vector4<Type>& interpolate(const t3Vector4<Type>& pnt, float p);

    T3_FORCE_INLINE t3Vector4<Type> getMiddle(const t3Vector4<Type>& pnt) const;

    T3_FORCE_INLINE t3Vector4<Type>& middle(const t3Vector4<Type>& pnt);

    T3_FORCE_INLINE t3Vector4<Type>& average(const t3Vector4<Type>* points, int num);

    T3_FORCE_INLINE t3Vector4<Type> getNormalized() const;

    T3_FORCE_INLINE t3Vector4<Type>& normalize();

    T3_FORCE_INLINE t3Vector4<Type> getLimited(float max) const;

    T3_FORCE_INLINE t3Vector4<Type>& limit(float max);

    T3_FORCE_INLINE float length() const;

    T3_FORCE_INLINE float lengthSquared() const;

    T3_FORCE_INLINE float dot(const t3Vector4<Type>& vector) const;

    // return all zero vector
    T3_FORCE_INLINE static t3Vector4<Type> zero() { return t3Vector4<Type>(0, 0, 0, 0); }

    // return all one vector
    T3_FORCE_INLINE static t3Vector4<Type> one() { return t3Vector4<Type>(1, 1, 1, 1); }

    // 测试使用的Console Log输出
    T3_FORCE_INLINE void print(std::string name) const;

    T3_FORCE_INLINE void print() const;
};


// Non-Member operators
template<typename Type>
inline t3Vector4<Type> operator+(float f, const t3Vector4<Type>& vec);
template<typename Type>
inline t3Vector4<Type> operator-(float f, const t3Vector4<Type>& vec);
template<typename Type>
inline t3Vector4<Type> operator*(float f, const t3Vector4<Type>& vec);
template<typename Type>
inline t3Vector4<Type> operator/(float f, const t3Vector4<Type>& vec);

#include <t3Math/core/t3Vector4.inl>

// 特化
typedef t3Vector4<float> t3Vector4f;
typedef t3Vector4<int> t3Vector4i;

// t2Point
typedef t3Vector4f t2Point4f;
typedef t3Vector4i t2Point4i;

//// 零向量
//static t3Vector4f t3Vector4fZero;
//static t3Vector4i t3Vector4iZero;

#endif