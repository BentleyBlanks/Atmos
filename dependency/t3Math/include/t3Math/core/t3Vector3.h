#ifndef T3_VECTOR3_H
#define T3_VECTOR3_H

#include <cmath>
#include <iostream>
#include <string>
#include <t3Math/core/t3Math.h>

template <class Type>
class t3Vector3
{
public:
	Type x, y, z;

	static const int DIM = 3;

	t3Vector3();
	t3Vector3(Type x, Type y, Type z);

	// assigns scalar to x, y, and z
	explicit t3Vector3(Type scalar);

	t3Vector3(const t3Vector3<Type>& vector);

    T3_FORCE_INLINE Type* getPtr() { return (Type*) &x; }

    T3_FORCE_INLINE const Type* getPtr() const { return (const Type *) &x; }

	inline Type& operator[](int n){ return getPtr()[n]; }

	inline Type operator[](int n) const{ return getPtr()[n]; }

	// Getters and Setters.
    T3_FORCE_INLINE void set(Type x, Type y, Type z);
    T3_FORCE_INLINE void set(const t3Vector3<Type>& vector);
    T3_FORCE_INLINE void set(Type scalar);

	// Check similarity/equality.
    inline bool operator==(const t3Vector3<Type>& vector);
	inline bool operator!=(const t3Vector3<Type>& vector) const;
	bool match(const t3Vector3<Type>& vector, float tolerance = 0.0001) const;

	// Operator overloading for t3Vector3
	// Not assert if Vector is dividing by zero
    inline t3Vector3<Type>  operator+(const t3Vector3<Type>& vector) const;
    inline t3Vector3<Type>& operator+=(const t3Vector3<Type>& vector);
    inline t3Vector3<Type>  operator-(const t3Vector3<Type>& vector) const;
    inline t3Vector3<Type>& operator-=(const t3Vector3<Type>& vector);
	// Dot Product
    inline Type operator*(const t3Vector3<Type>& vector) const;
    // *= Not Exist
	//t3Vector3<Type>& operator*=(const t3Vector3<Type>& vector);
	// Not valid in mathmatics
    inline t3Vector3<Type>  operator/(const t3Vector3<Type>& vector) const;
    inline t3Vector3<Type>& operator/=(const t3Vector3<Type>& vector);
    inline t3Vector3<Type>  operator-() const;

	// Operator overloading for <Type>
    inline t3Vector3<Type>  operator+(const Type scalar) const;
    inline t3Vector3<Type>& operator+=(const Type scalar);
    inline t3Vector3<Type>  operator-(const Type scalar) const;
    inline t3Vector3<Type>& operator-=(const Type scalar);
    inline t3Vector3<Type>  operator*(const Type scalar) const;
    inline t3Vector3<Type>& operator*=(const Type scalar);
    inline t3Vector3<Type>  operator/(const Type scalar) const;
    inline t3Vector3<Type>& operator/=(const Type scalar);

	//--! Not Avaliable
	//friend ostream& operator<<(ostream& os, const t3Vector3<Type>& vector);
	//friend istream& operator>>(istream& is, t3Vector3<Type>& vector);

	// Get vector same direction with length parameter.
    T3_FORCE_INLINE t3Vector3<Type> getScaled(const float length) const;
    T3_FORCE_INLINE t3Vector3<Type> scale(const float length);

	// Rotation
	//--! Not Avaliable
    T3_FORCE_INLINE t3Vector3<Type>  getRotated(float angle, const t3Vector3<Type>& axis) const {}
    T3_FORCE_INLINE t3Vector3<Type>  getRotatedRad(float angle, const t3Vector3<Type>& axis) const {}
    T3_FORCE_INLINE t3Vector3<Type>& rotate(float angle, const t3Vector3<Type>& axis) {}
    T3_FORCE_INLINE t3Vector3<Type>& rotateRad(float angle, const t3Vector3<Type>& axis) {}
    T3_FORCE_INLINE t3Vector3<Type>  getRotated(float ax, float ay, float az) const {}
    T3_FORCE_INLINE t3Vector3<Type>  getRotatedRad(float ax, float ay, float az) const {}
    T3_FORCE_INLINE t3Vector3<Type>& rotate(float ax, float ay, float az) {}
    T3_FORCE_INLINE t3Vector3<Type>& rotateRad(float ax, float ay, float az) {}


	// Rotation,point around pivot
	//--! Not Avaliable
    T3_FORCE_INLINE t3Vector3<Type>   getRotated(float angle, const t3Vector3<Type>& pivot, const t3Vector3<Type>& axis) const {}
    T3_FORCE_INLINE t3Vector3<Type>&  rotate(float angle, const t3Vector3<Type>& pivot, const t3Vector3<Type>& axis) {}
    T3_FORCE_INLINE t3Vector3<Type>   getRotatedRad(float angle, const t3Vector3<Type>& pivot, const t3Vector3<Type>& axis) const {}
    T3_FORCE_INLINE t3Vector3<Type>&  rotateRad(float angle, const t3Vector3<Type>& pivot, const t3Vector3<Type>& axis) {}

	// Map point to coordinate system defined by origin, vx, vy, and vz.
	//--! Not Avaliable
    T3_FORCE_INLINE t3Vector3<Type> getMapped(const t3Vector3<Type>& origin, const t3Vector3<Type>& vx, const t3Vector3<Type>& vy, const t3Vector3<Type>& vz) const
	{}
    T3_FORCE_INLINE t3Vector3<Type>& map(const t3Vector3<Type>& origin, const t3Vector3<Type>& vx, const t3Vector3<Type>& vy, const t3Vector3<Type>& vz) {}

	// Distance between two points.
    T3_FORCE_INLINE float distance(const t3Vector3<Type>& point) const;
    T3_FORCE_INLINE float squareDistance(const t3Vector3<Type>& point) const;

	// Linear interpolation.
    T3_FORCE_INLINE t3Vector3<Type> getInterpolated(const t3Vector3<Type>& point, float t) const;
    T3_FORCE_INLINE t3Vector3<Type>& interpolate(const t3Vector3<Type>& point, float t);
    T3_FORCE_INLINE t3Vector3<Type> getMiddle(const t3Vector3<Type>& point);
    T3_FORCE_INLINE t3Vector3<Type>& middle(const t3Vector3<Type>& point);
	// Average (centroid) among points.
    T3_FORCE_INLINE t3Vector3<Type>& average(const t3Vector3<Type>* points, int num);

	// Normalization
    T3_FORCE_INLINE t3Vector3<Type> getNormalized() const;
    T3_FORCE_INLINE t3Vector3<Type>& normalize();

	// Limit length.
    T3_FORCE_INLINE t3Vector3<Type>  getLimited(float max) const;
    T3_FORCE_INLINE t3Vector3<Type>& limit(float max);

	// Perpendicular vector.
    T3_FORCE_INLINE t3Vector3<Type>  getCrossed(const t3Vector3<Type>& vector) const;
    T3_FORCE_INLINE t3Vector3<Type>& cross(const t3Vector3<Type>& vector);
	// Normalized Perpendicular vector
    T3_FORCE_INLINE t3Vector3<Type>  getPerpendicular(const t3Vector3<Type>& vector) const;
    T3_FORCE_INLINE t3Vector3<Type>& perpendicular(const t3Vector3<Type>& vector);

	// Length
    T3_FORCE_INLINE float length() const;
    T3_FORCE_INLINE float lengthSquared() const;

	// Angle between two vectors
    T3_FORCE_INLINE float angle(const t3Vector3<Type>& vector) const;
    T3_FORCE_INLINE float angleRad(const t3Vector3<Type>& vector) const;

	// Dot Product
    T3_FORCE_INLINE float dot(const t3Vector3<Type>& vector) const;

	// return all zero vector
    T3_FORCE_INLINE static t3Vector3<Type> zero() { return t3Vector3<float>(0, 0, 0); }
	// return all zero vector
    T3_FORCE_INLINE static t3Vector3<Type> one() { return t3Vector3<float>(1, 1, 1); }

	// 测试使用的Console Log输出
    T3_FORCE_INLINE void print(std::string name) const;

    T3_FORCE_INLINE void print() const;
};


//// Non-Member operators
//template<typename Type>
//t3Vector3<Type> operator+(float f, const t3Vector3<Type>& vec);
//
//template<typename Type>
//t3Vector3<Type> operator-(float f, const t3Vector3<Type>& vec);
//
//template<typename Type>
//t3Vector3<Type> operator*(float f, const t3Vector3<Type>& vec);
//
//template<typename Type>
//t3Vector3<Type> operator/(float f, const t3Vector3<Type>& vec);
// Non-Member operators

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> operator+(float scalar, const t3Vector3<Type>& vector)
{
    return t3Vector3<Type>(scalar + vector.x, scalar + vector.y, scalar + vector.z);
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> operator-(float scalar, const t3Vector3<Type>& vector)
{
    return t3Vector3<Type>(scalar - vector.x, scalar - vector.y, scalar - vector.z);
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> operator*(float scalar, const t3Vector3<Type>& vector)
{
    return t3Vector3<Type>(scalar*vector.x, scalar*vector.y, scalar*vector.z);
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> operator/(float scalar, const t3Vector3<Type>& vector)
{
    return t3Vector3<Type>(scalar / vector.x, scalar / vector.y, scalar / vector.z);
}

#include <t3Math/core/t3Vector3.inl>

// 特化
typedef t3Vector3<float> t3Vector3f;
typedef t3Vector3<int> t3Vector3i;

// t2Point
typedef t3Vector3f t2Point3f;
typedef t3Vector3i t2Point3i;

//// 零向量
//static t3Vector3f t3Vector3fZero;
//static t3Vector3i t3Vector3iZero;

#endif