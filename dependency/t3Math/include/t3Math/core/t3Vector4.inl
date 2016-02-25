#ifndef T3_VECTOR4_INL
#define T3_VECTOR4_INL

#include <t3Math/core/t3Vector4.h>

template<typename Type>
inline t3Vector4<Type>::t3Vector4() : x(0), y(0), z(0), w(0) {}
template<typename Type>
inline t3Vector4<Type>::t3Vector4(Type x, Type y, Type z, Type w) : x(x), y(y), z(z), w(w) {}
template<typename Type>
inline t3Vector4<Type>::t3Vector4(Type scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
template <class Type>
inline t3Vector4<Type>::t3Vector4(const t3Vector4& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}

// Getters and Setters.
template<typename Type>
T3_FORCE_INLINE void t3Vector4<Type>::set(Type _scalar)
{
    x = _scalar;
    y = _scalar;
    z = _scalar;
    w = _scalar;
}

template<typename Type>
T3_FORCE_INLINE void t3Vector4<Type>::set(Type _x, Type _y, Type _z, Type _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

template<typename Type>
T3_FORCE_INLINE void t3Vector4<Type>::set(const t3Vector4<Type>& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = vec.w;
}


// Check similarity/equality.
template<typename Type>
inline bool t3Vector4<Type>::operator==(const t3Vector4<Type>& vec) const
{
    return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
}

template<typename Type>
inline bool t3Vector4<Type>::operator!=(const t3Vector4<Type>& vec) const
{
    return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
}

template<typename Type>
T3_FORCE_INLINE bool t3Vector4<Type>::match(const t3Vector4<Type>& vec, float tolerance) const
{
    return (fabs(x - vec.x) < tolerance)
        && (fabs(y - vec.y) < tolerance)
        && (fabs(z - vec.z) < tolerance)
        && (fabs(w - vec.w) < tolerance);
}

// Additions and Subtractions.
template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator+(const t3Vector4<Type>& vec) const
{
    return t3Vector4<Type>(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
}

template<typename Type>
inline t3Vector4<Type>& t3Vector4<Type>::operator+=(const t3Vector4<Type>& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
    w += vec.w;
    return *this;
}

template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator-(const Type f) const
{
    return t3Vector4<Type>(x - f, y - f, z - f, w - f);
}

template<typename Type>
inline t3Vector4<Type>& t3Vector4<Type>::operator-=(const Type f)
{
    x -= f;
    y -= f;
    z -= f;
    w -= f;
    return *this;
}

template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator-(const t3Vector4<Type>& vec) const
{
    return t3Vector4<Type>(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

template<typename Type>
inline t3Vector4<Type>& t3Vector4<Type>::operator-=(const t3Vector4<Type>& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    w -= vec.w;
    return *this;
}

template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator+(const Type f) const
{
    return t3Vector4<Type>(x + f, y + f, z + f, w + f);
}

template<typename Type>
inline t3Vector4<Type>& t3Vector4<Type>::operator+=(const Type f)
{
    x += f;
    y += f;
    z += f;
    w += f;
    return *this;
}

template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator-() const
{
    return t3Vector4<Type>(-x, -y, -z, -w);
}

// Scalings
template<typename Type>
inline Type t3Vector4<Type>::operator*(const t3Vector4<Type>& vec) const
{
    return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}

//template<typename Type>
//inline t3Vector4<Type>& t3Vector4<Type>::operator*=(const t3Vector4<Type>& vec)
//{
//    x *= vec.x;
//    y *= vec.y;
//    z *= vec.z;
//    w *= vec.w;
//    return *this;
//}

template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator*(const Type f) const
{
    return t3Vector4<Type>(x*f, y*f, z*f, w*f);
}

template<typename Type>
inline t3Vector4<Type>& t3Vector4<Type>::operator*=(const Type f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator/(const t3Vector4<Type>& vec) const
{
    return t3Vector4<Type>(vec.x != 0 ? x / vec.x : x, vec.y != 0 ? y / vec.y : y, vec.z != 0 ? z / vec.z : z, vec.w != 0 ? w / vec.w : w);
}

template<typename Type>
inline t3Vector4<Type>& t3Vector4<Type>::operator/=(const t3Vector4<Type>& vec)
{
    vec.x != 0 ? x /= vec.x : x;
    vec.y != 0 ? y /= vec.y : y;
    vec.z != 0 ? z /= vec.z : z;
    vec.w != 0 ? w /= vec.w : w;
    return *this;
}

template<typename Type>
inline t3Vector4<Type> t3Vector4<Type>::operator/(const Type f) const
{
    if(f == 0) return t3Vector4<Type>(x, y, z, w);

    return t3Vector4<Type>(x / f, y / f, z / f, w / f);
}

template<typename Type>
inline t3Vector4<Type>& t3Vector4<Type>::operator/=(const Type f)
{
    if(f == 0)return *this;

    x /= f;
    y /= f;
    z /= f;
    w /= f;
    return *this;
}

template<typename Type>
T3_FORCE_INLINE t3Vector4<Type> t3Vector4<Type>::getScaled(const float length) const
{
    float l = (float) sqrt(x*x + y*y + z*z + w*w);
    if(l > 0)
        return t3Vector4<Type>((x / l)*length, (y / l)*length,
        (z / l)*length, (w / l)*length);
    else
        return t3Vector4<Type>();
}

template<typename Type>
T3_FORCE_INLINE t3Vector4<Type>& t3Vector4<Type>::scale(const float length)
{
    float l = (float) sqrt(x*x + y*y + z*z + w*w);
    if(l > 0)
    {
        x = (x / l)*length;
        y = (y / l)*length;
        z = (z / l)*length;
        w = (w / l)*length;
    }
    return *this;
}



// Distance between two points.
//

template<typename Type>//
T3_FORCE_INLINE float t3Vector4<Type>::distance(const t3Vector4<Type>& pnt) const
{
    float vx = x - pnt.x;
    float vy = y - pnt.y;
    float vz = z - pnt.z;
    float vw = w - pnt.w;
    return (float) sqrt(vx*vx + vy*vy + vz*vz + vw*vw);
}

template<typename Type>
T3_FORCE_INLINE float t3Vector4<Type>::squareDistance(const t3Vector4<Type>& pnt) const
{
    float vx = x - pnt.x;
    float vy = y - pnt.y;
    float vz = z - pnt.z;
    float vw = w - pnt.w;
    return vx*vx + vy*vy + vz*vz + vw*vw;
}



// Linear interpolation.
//
//
/**
* p==0.0 results in this point, p==0.5 results in the
* midpoint, and p==1.0 results in pnt being returned.
*/
template<typename Type>
T3_FORCE_INLINE t3Vector4<Type> t3Vector4<Type>::getInterpolated(const t3Vector4<Type>& pnt, float p) const
{
    return t3Vector4<Type>(x*(1 - p) + pnt.x*p,
                           y*(1 - p) + pnt.y*p,
                           z*(1 - p) + pnt.z*p,
                           w*(1 - p) + pnt.w*p);
}

template<typename Type>
T3_FORCE_INLINE t3Vector4<Type>& t3Vector4<Type>::interpolate(const t3Vector4<Type>& pnt, float p)
{
    x = x*(1 - p) + pnt.x*p;
    y = y*(1 - p) + pnt.y*p;
    z = z*(1 - p) + pnt.z*p;
    w = w*(1 - p) + pnt.w*p;
    return *this;
}

template<typename Type>
T3_FORCE_INLINE t3Vector4<Type> t3Vector4<Type>::getMiddle(const t3Vector4<Type>& pnt) const
{
    return t3Vector4<Type>((x + pnt.x) / 2.0f, (y + pnt.y) / 2.0f,
                           (z + pnt.z) / 2.0f, (w + pnt.w) / 2.0f);
}

template<typename Type>
T3_FORCE_INLINE t3Vector4<Type>& t3Vector4<Type>::middle(const t3Vector4<Type>& pnt)
{
    x = (x + pnt.x) / 2.0f;
    y = (y + pnt.y) / 2.0f;
    z = (z + pnt.z) / 2.0f;
    w = (w + pnt.w) / 2.0f;
    return *this;
}


// Average (centroid) among points.
// (Addition is sometimes useful for calculating averages too)
//

template<typename Type>//
T3_FORCE_INLINE t3Vector4<Type>& t3Vector4<Type>::average(const t3Vector4<Type>* points, int num)
{
    x = 0.f;
    y = 0.f;
    z = 0.f;
    w = 0.f;
    for(int i = 0; i < num; i++)
    {
        x += points[i].x;
        y += points[i].y;
        z += points[i].z;
        w += points[i].w;
    }
    x /= num;
    y /= num;
    z /= num;
    w /= num;
    return *this;
}



// Normalization
template<typename Type>
T3_FORCE_INLINE t3Vector4<Type> t3Vector4<Type>::getNormalized() const
{
    float length = (float) sqrt(x*x + y*y + z*z + w*w);
    if(length > 0)
    {
        return t3Vector4<Type>(x / length, y / length, z / length, w / length);
    }
    else
    {
        return t3Vector4<Type>();
    }
}

template<typename Type>
T3_FORCE_INLINE t3Vector4<Type>& t3Vector4<Type>::normalize()
{
    float lenght = (float) sqrt(x*x + y*y + z*z + w*w);
    if(lenght > 0)
    {
        x /= lenght;
        y /= lenght;
        z /= lenght;
        w /= lenght;
    }
    return *this;
}



// Limit length.
template<typename Type>
T3_FORCE_INLINE t3Vector4<Type> t3Vector4<Type>::getLimited(float max) const
{
    t3Vector4<Type> limited;
    float lengthSquared = (x*x + y*y + z*z + w*w);
    if(lengthSquared > max*max && lengthSquared > 0)
    {
        float ratio = max / (float) sqrt(lengthSquared);
        limited.set(x*ratio, y*ratio, z*ratio, w*ratio);
    }
    else
    {
        limited.set(x, y, z, w);
    }
    return limited;
}

template<typename Type>
T3_FORCE_INLINE t3Vector4<Type>& t3Vector4<Type>::limit(float max)
{
    float lengthSquared = (x*x + y*y + z*z + w*w);
    if(lengthSquared > max*max && lengthSquared > 0)
    {
        float ratio = max / (float) sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
        w *= ratio;
    }
    return *this;
}



// Length
template<typename Type>//
T3_FORCE_INLINE float t3Vector4<Type>::length() const
{
    return (float) sqrt(x*x + y*y + z*z + w*w);
}

template<typename Type>
T3_FORCE_INLINE float t3Vector4<Type>::lengthSquared() const
{
    return (float) (x*x + y*y + z*z + w*w);
}




/**
* Dot Product.
*/
template<typename Type>
T3_FORCE_INLINE float t3Vector4<Type>::dot(const t3Vector4<Type>& vec) const
{
    return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}


template <class Type>
T3_FORCE_INLINE void t3Vector4<Type>::print() const
{
#ifdef _DEBUG
    std::cout << "t3Vector4 ";
    std::cout << "(x: " << x << ", y: " << y << ", z:" << z << ", w:" << w << ")" << std::endl;
#endif
}

template <class Type>
T3_FORCE_INLINE void t3Vector4<Type>::print(std::string name) const
{
#ifdef _DEBUG
    // test name null
    if(name.empty())
        name = "";
    std::cout << "t3Vector4 ";
    std::cout << name << "(x: " << x << ", y: " << y << ", z:" << z  << ", w:" << w << ")" << std::endl;
#endif
}

// Non-Member operators
template<typename Type>//
inline t3Vector4<Type> operator+(float f, const t3Vector4<Type>& vec)
{
    return t3Vector4<Type>(f + vec.x, f + vec.y, f + vec.z, f + vec.w);
}

template<typename Type>
inline t3Vector4<Type> operator-(float f, const t3Vector4<Type>& vec)
{
    return t3Vector4<Type>(f - vec.x, f - vec.y, f - vec.z, f - vec.w);
}

template<typename Type>
inline t3Vector4<Type> operator*(float f, const t3Vector4<Type>& vec)
{
    return t3Vector4<Type>(f*vec.x, f*vec.y, f*vec.z, f*vec.w);
}

template<typename Type>
inline t3Vector4<Type> operator/(float f, const t3Vector4<Type>& vec)
{
    return t3Vector4<Type>(f / vec.x, f / vec.y, f / vec.z, f / vec.w);
}

#endif