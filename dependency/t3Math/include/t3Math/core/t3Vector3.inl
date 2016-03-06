#ifndef T3_VECTOR3_INL
#define T3_VECTOR3_INL

#include <t3Math/core/t3Vector3.h>


// -----------------------Implementation-----------------------
template<typename Type>
inline t3Vector3<Type>::t3Vector3() : x(0), y(0), z(0) {}
template<typename Type>
inline t3Vector3<Type>::t3Vector3(Type x, Type y, Type z) : x(x), y(y), z(z) {}
template<typename Type>
inline t3Vector3<Type>::t3Vector3(Type scalar) : x(scalar), y(scalar), z(scalar) {}
template<typename Type>
inline t3Vector3<Type>::t3Vector3(const t3Vector3<Type>& vector) : x(vector.x), y(vector.y), z(vector.z) {}


// Getters and Setters.
template<typename Type>
inline void t3Vector3<Type>::set(Type x, Type y, Type z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

template<typename Type>
inline void t3Vector3<Type>::set(const t3Vector3<Type>& vector)
{
    x = vector.x;
    y = vector.y;
    z = vector.z;
}

template<typename Type>
inline void t3Vector3<Type>::set(Type scalar)
{
    x = scalar;
    y = scalar;
    z = scalar;
}

// Check similarity/equality.
template<typename Type>
inline bool t3Vector3<Type>::operator==(const t3Vector3<Type>& vector) const
{
    return (x == vector.x) && (y == vector.y) && (z == vector.z);
}

template<typename Type>
inline bool t3Vector3<Type>::operator!=(const t3Vector3<Type>& vector) const
{
    return (x != vector.x) || (y != vector.y) || (z != vector.z);
}

template<typename Type>
T3_FORCE_INLINE bool t3Vector3<Type>::match(const t3Vector3<Type>& vector, float tolerance/* = 0.0001*/) const
{
    return (fabs(x - vector.x) < tolerance)
        && (fabs(y - vector.y) < tolerance)
        && (fabs(z - vector.z) < tolerance);
}

// Operator overloading for t3Vector3
template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator+(const t3Vector3<Type>& vector) const
{
    return t3Vector3<Type>(x + vector.x, y + vector.y, z + vector.z);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator+=(const t3Vector3<Type>& vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator-(const t3Vector3<Type>& vector) const
{
    return t3Vector3<Type>(x - vector.x, y - vector.y, z - vector.z);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator-=(const t3Vector3<Type>& vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator*(const t3Vector3<Type>& vector) const
{
    return t3Vector3<Type>(x*vector.x + y*vector.y + z*vector.z);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator*=(const t3Vector3<Type>& vector)
{
	x *= vector.x;
	y *= vector.y;
	z *= vector.z;
	return *this;
}

template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator/(const t3Vector3<Type>& vector) const
{
    assert(vector.x != 0 && vector.y != 0 && vector.z != 0);

    return t3Vector3<Type>(x / vector.x, y / vector.y, z / vector.z);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator/=(const t3Vector3<Type>& vector)
{
    assert(vector.x != 0 && vector.y != 0 && vector.z != 0);

    x /= vector.x;
    y /= vector.y;
    z /= vector.z;

    return *this;
}

template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator-() const
{
    return t3Vector3<Type>(-x, -y, -z);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator=(const t3Vector3<Type>& vector)
{
    x = vector.x;
    y = vector.y;
    z = vector.z;
    return *this;
}

// operator overloading for float
template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator+(const Type scalar) const
{
    return t3Vector3<Type>(x + scalar, y + scalar, z + scalar);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator+=(const Type scalar)
{
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
}

template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator-(const Type scalar) const
{
    return t3Vector3<Type>(x - scalar, y - scalar, z - scalar);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator-=(const Type scalar)
{
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
}

template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator*(const Type scalar) const
{
    return t3Vector3<Type>(x*scalar, y*scalar, z*scalar);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator*=(const Type scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

template<typename Type>
inline t3Vector3<Type> t3Vector3<Type>::operator/(const Type scalar) const
{
    assert(scalar != 0);

    return t3Vector3<Type>(x / scalar, y / scalar, z / scalar);
}

template<typename Type>
inline t3Vector3<Type>& t3Vector3<Type>::operator/=(const Type scalar)
{
    assert(scalar != 0);

    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

// Get vector same direction with length parameter.
template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::getScaled(const float length) const
{
    float l = (float) sqrt(x*x + y*y + z*z);
    if(l > 0)
        return t3Vector3<Type>((x / l)*length, (y / l)*length, (z / l)*length);
    else
        return t3Vector3<Type>();
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::scale(const float length)
{
    float l = (float) sqrt(x*x + y*y + z*z);
    if(l > 0)
    {
        x = (x / l)*length;
        y = (y / l)*length;
        z = (z / l)*length;
    }
    return *this;
}

// Distance between two points.
template<typename Type>
T3_FORCE_INLINE float t3Vector3<Type>::distance(const t3Vector3<Type>& point) const
{
    float vx = x - point.x;
    float vy = y - point.y;
    float vz = z - point.z;
    return (float) sqrt(vx*vx + vy*vy + vz*vz);
}

template<typename Type>
T3_FORCE_INLINE float t3Vector3<Type>::squareDistance(const t3Vector3<Type>& point) const
{
    float vx = x - point.x;
    float vy = y - point.y;
    float vz = z - point.z;
    return vx*vx + vy*vy + vz*vz;
}

// Linear interpolation.
template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::getInterpolated(const t3Vector3<Type>& point, float t) const
{
    return t3Vector3<Type>(x*(1 - t) + point.x*t, y*(1 - t) + point.y*t, z*(1 - t) + point.z*t);
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type>& t3Vector3<Type>::interpolate(const t3Vector3<Type>& point, float t)
{
    x = x*(1 - t) + point.x*t;
    y = y*(1 - t) + point.y*t;
    z = z*(1 - t) + point.z*t;
    return *this;
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::getMiddle(const t3Vector3<Type>& point)
{
    return t3Vector3<Type>((x + point.x) * 0.5f, (y + point.y) * 0.5f, (z + point.z) * 0.5f);
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type>& t3Vector3<Type>::middle(const t3Vector3<Type>& point)
{
    x = (x + point.x) * 0.5f;
    y = (y + point.y) * 0.5f;
    z = (z + point.z) * 0.5f;
    return *this;
}

// Average (centroid) among points.
template<typename Type>
T3_FORCE_INLINE t3Vector3<Type>& t3Vector3<Type>::average(const t3Vector3<Type>* points, int num)
{
    x = 0.f;
    y = 0.f;
    z = 0.f;
    for(int i = 0; i < num; i++)
    {
        x += points[i].x;
        y += points[i].y;
        z += points[i].z;
    }
    x /= num;
    y /= num;
    z /= num;
    return *this;
}

// Normalization
template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::getNormalized() const
{
    float length = (float) sqrt(x*x + y*y + z*z);
    if(length > 0)
        return t3Vector3<Type>(x / length, y / length, z / length);
    else
        return t3Vector3<Type>();
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type>& t3Vector3<Type>::normalize()
{
    float length = (float) sqrt(x*x + y*y + z*z);
    if(length > 0)
    {
        x /= length;
        y /= length;
        z /= length;
    }
    return *this;
}

// Limit length.
template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::getLimited(float max) const
{
    t3Vector3<Type> limited;
    float lengthSquared = (x*x + y*y + z*z);
    if(lengthSquared > max*max && lengthSquared > 0)
    {
        float ratio = max / (float) sqrt(lengthSquared);
        limited.set(x*ratio, y*ratio, z*ratio);
    }
    else
        limited.set(x, y, z);

    return limited;
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type>& t3Vector3<Type>::limit(float max)
{
    float lengthSquared = (x*x + y*y + z*z);
    if(lengthSquared > max*max && lengthSquared > 0)
    {
        float ratio = max / (float) sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
    }
    return *this;
}

// Perpendicular vector.
template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::getCrossed(const t3Vector3<Type>& vector) const
{
    return t3Vector3<Type>(y*vector.z - z*vector.y,
                           z*vector.x - x*vector.z,
                           x*vector.y - y*vector.x);
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type>& t3Vector3<Type>::cross(const t3Vector3<Type>& vector)
{
    float tempX = y*vector.z - z*vector.y;
    float tempY = z*vector.x - x*vector.z;

    // component z would get changed between computation, what would made the computation of x and y became a mistake.
    z = x*vector.y - y*vector.x;
    x = tempX;
    y = tempY;

    return *this;
}

// Normalized perpendicular.
template<typename Type>
T3_FORCE_INLINE t3Vector3<Type> t3Vector3<Type>::getPerpendicular(const t3Vector3<Type>& vector) const
{
    float crossX = y*vector.z - z*vector.y;
    float crossY = z*vector.x - x*vector.z;
    float crossZ = x*vector.y - y*vector.x;

    float length = (float) sqrt(crossX*crossX + crossY*crossY + crossZ*crossZ);

    if(length > 0)
        return t3Vector3<Type>(crossX / length, crossY / length, crossZ / length);
    else
        return t3Vector3<Type>();
}

template<typename Type>
T3_FORCE_INLINE t3Vector3<Type>& t3Vector3<Type>::perpendicular(const t3Vector3<Type>& vector)
{
    float crossX = y*vector.z - z*vector.y;
    float crossY = z*vector.x - x*vector.z;
    float crossZ = x*vector.y - y*vector.x;

    float length = (float) sqrt(crossX*crossX + crossY*crossY + crossZ*crossZ);

    if(length > 0)
    {
        x = crossX / length;
        y = crossY / length;
        z = crossZ / length;
    }
    else
    {
        x = 0.f;
        y = 0.f;
        z = 0.f;
    }

    return *this;
}

// Length
template<typename Type>
T3_FORCE_INLINE float t3Vector3<Type>::length() const
{
    return (float) sqrt(x*x + y*y + z*z);
}

template<typename Type>
T3_FORCE_INLINE float t3Vector3<Type>::lengthSquared() const
{
    return (float) (x*x + y*y + z*z);
}

/**
* Angle (deg) between two vectors.
* This is an unsigned relative angle from 0 to 180.
* http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
*/
template<typename Type>
T3_FORCE_INLINE float t3Vector3<Type>::angle(const t3Vector3<Type>& vector) const
{
    t3Vector3<Type> n1 = this->normalized();
    t3Vector3<Type> n2 = vector.normalized();
    return (float) (acos(n1.dot(n2))*RAD_TO_DEG);
}

template<typename Type>
T3_FORCE_INLINE float t3Vector3<Type>::angleRad(const t3Vector3<Type>& vector) const
{
    t3Vector3<Type> n1 = this->normalized();
    t3Vector3<Type> n2 = vector.normalized();
    return (float) acos(n1.dot(n2));
}

// Dot Product
template<typename Type>
T3_FORCE_INLINE float t3Vector3<Type>::dot(const t3Vector3<Type>& vector) const
{
    return x*vector.x + y*vector.y + z*vector.z;
}

// For test to print in console
template<typename Type>
T3_FORCE_INLINE void t3Vector3<Type>::print(std::string name) const
{
#ifdef _DEBUG
    // test name null
    if(name.empty())
        name = "";
    std::cout << "t3Vector3 ";
    std::cout << name << "(x: " << x << ", y: " << y << ", z:" << z << ")" << std::endl;
#endif
}

template<typename Type>
T3_FORCE_INLINE void t3Vector3<Type>::print() const
{
#ifdef _DEBUG
    std::cout << "t3Vector3 ";
    std::cout << "(x: " << x << ", y: " << y << ", z:" << z << ")" << std::endl;
#endif
}

//// Non-Member operators
//template<typename Type>
//T3_FORCE_INLINE t3Vector3<Type> operator+(float scalar, const t3Vector3<Type>& vector)
//{
//    return t3Vector3<Type>(scalar + vector.x, scalar + vector.y, scalar + vector.z);
//}
//
//template<typename Type>
//T3_FORCE_INLINE t3Vector3<Type> operator-(float scalar, const t3Vector3<Type>& vector)
//{
//    return t3Vector3<Type>(scalar - vector.x, scalar - vector.y, scalar - vector.z);
//}
//
//template<typename Type>
//T3_FORCE_INLINE t3Vector3<Type> operator*(float scalar, const t3Vector3<Type>& vector)
//{
//    return t3Vector3<Type>(scalar*vector.x, scalar*vector.y, scalar*vector.z);
//}
//
//template<typename Type>
//T3_FORCE_INLINE t3Vector3<Type> operator/(float scalar, const t3Vector3<Type>& vector)
//{
//    return t3Vector3<Type>(scalar / vector.x, scalar / vector.y, scalar / vector.z);
//}

#endif