#ifndef T3_QUATERNION_H
#define T3_QUATERNION_H

#include <t3Math/core/t3Vector3.h>
#include <t3Math/core/t3Vector4.h>
#include <cmath>

#if (_MSC_VER)       
// make microsoft visual studio complain less about double / float conversion.
#pragma warning(disable : 4244)
#endif

class t3Matrix4x4;

class t3Quaternion
{
public:
    //    float _v[4];
    /// \cond INTERNAL
    t3Vector4f _v;
    /// \endcond



    //---------------------
    /// \name Constructor
    /// \{

    inline t3Quaternion();
    inline t3Quaternion(float x, float y, float z, float w);
    inline t3Quaternion(const t3Vector4f& v);
    inline t3Quaternion(float angle, const t3Vector3f& axis);

    // rotation order is axis3,axis2,axis1
    inline t3Quaternion(float angle1, const t3Vector3f& axis1, float angle2, const t3Vector3f& axis2, float angle3, const t3Vector3f& axis3);


    /// \}

    //---------------------
    /// \name Setters
    /// \{

    inline void set(float x, float y, float z, float w);
    inline void set(const t3Vector4f& v);

    void set(const t3Matrix4x4& matrix);


    /// \}

    //---------------------
    /// \name Getters
    /// \{


    inline float& operator [](int i);
    inline float operator [](int i) const;

    void get(t3Matrix4x4& matrix) const;


    inline float& x();
    inline float& y();
    inline float& z();
    inline float& w();

    inline float x() const;
    inline float y() const;
    inline float z() const;
    inline float w() const;

    inline t3Vector4f asVec4() const;
    inline t3Vector3f asVec3() const;


    /// \}

    //---------------------
    /// \name Functions
    /// \{


    /// \brief return true if the Quat represents a zero rotation, 
    /// and therefore can be ignored in computations.
    inline bool zeroRotation() const;


    /// \brief Length of the quaternion = sqrt(vec . vec)
    inline float length() const;

    /// \brief Length of the quaternion = vec . vec
    inline float length2() const;

    /// \brief Conjugate
    inline t3Quaternion conj() const;

    /// \brief Multiplicative inverse method
    ///
    ///     q^(-1) = q^*/(q.q^*)
    inline const t3Quaternion inverse() const;



    /// \briefSet a quaternion which will perform a rotation of an
    /// angle around the axis given by the vector(x,y,z).
    ///
    /// Define Spherical Linear interpolation method also
    void makeRotate(float angle, float x, float y, float z);
    void makeRotate(float angle, const t3Vector3f& vec);
    void makeRotate(float angle1, const t3Vector3f& axis1, float angle2, const t3Vector3f& axis2, float angle3, const t3Vector3f& axis3);


    /// \brief Make a rotation Quat which will rotate vec1 to vec2.
    /// Generally take a dot product to get the angle between these
    /// and then use a cross product to get the rotation axis
    /// Watch out for the two special cases when the vectors
    /// are co-incident or opposite in direction.
    void makeRotate(const t3Vector3f& vec1, const t3Vector3f& vec2);

    void makeRotate_original(const t3Vector3f& vec1, const t3Vector3f& vec2);

    /// \brief Return the angle and vector components represented by the quaternion.
    void getRotate(float&angle, float& x, float& y, float& z) const;
    void getRotate(float& angle, t3Vector3f& vec) const;

    /// \brief Calculate and return the rotation as euler angles
    t3Vector3f getEuler() const;


    /// \brief Spherical Linear Interpolation.
    ///
    /// As t goes from 0 to 1, the Quat object goes from "from" to "to".
    void slerp(float t, const t3Quaternion& from, const t3Quaternion& to);

    inline void normalize();

    /// \}

    //---------------------
    /// \name Operators
    /// \{


    // Implemented in terms of Vec4s. Some Vec4 operators, e.g.
    // operator* are not appropriate for quaternions (as
    // mathematical objects) so they are implemented differently.
    // Also define methods for conjugate and the multiplicative inverse.

    inline t3Quaternion& operator =(const t3Quaternion& q);
    inline bool operator ==(const t3Quaternion& q) const;
    inline bool operator !=(const t3Quaternion& q) const;
    //    inline bool operator <(const t3Quaternion& q) const;  // why?


    inline const t3Quaternion operator *(float rhs) const;                  ///< Multiply by scalar
    inline const t3Quaternion operator*(const t3Quaternion& rhs) const;     ///< Binary multiply
    inline t3Vector3f operator*(const t3Vector3f& v) const;                       ///< Rotate a vector by this quaternion.
    inline t3Quaternion& operator *=(float rhs);                            ///< Unary multiply by scalar
    inline t3Quaternion& operator*=(const t3Quaternion& rhs);               ///< Unary multiply
    inline t3Quaternion operator /(float rhs) const;                        ///< Divide by scalar
    inline const t3Quaternion operator/(const t3Quaternion& denom) const;   ///< Binary divide
    inline t3Quaternion& operator /=(float rhs);                            ///< Unary divide by scalar
    inline t3Quaternion& operator/=(const t3Quaternion& denom);             ///< Unary divide
    inline const t3Quaternion operator +(const t3Quaternion& rhs) const;    ///< Binary addition
    inline t3Quaternion& operator +=(const t3Quaternion& rhs);              ///< Unary addition
    inline t3Quaternion& operator -=(const t3Quaternion& rhs);              ///< Unary subtraction
    inline const t3Quaternion operator -(const t3Quaternion& rhs) const;    ///< Binary subtraction
    inline const t3Quaternion operator -() const;                           ///< returns the negative of the quaternion. calls operator -() on the Vec4

    inline std::ostream& operator<<(std::ostream& os);
    inline std::istream& operator>>(std::istream& is);

    /// \}
};



// ----------------------------------------------------------------
// IMPLEMENTATION
// ----------------------------------------------------------------



//----------------------------------------
t3Quaternion::t3Quaternion()
{
    _v.set(0, 0, 0, 1);
}


//----------------------------------------
std::ostream& t3Quaternion::operator<<(std::ostream& os)
{
    os << _v.x << ", " << _v.y << ", " << _v.z << ", " << _v.w;
    return os;
}


//----------------------------------------
std::istream& t3Quaternion::operator>>(std::istream& is)
{
    is >> _v.x;
    is.ignore(2);
    is >> _v.y;
    is.ignore(2);
    is >> _v.z;
    is.ignore(2);
    is >> _v.w;
    return is;
}


//----------------------------------------
t3Quaternion::t3Quaternion(float x, float y, float z, float w)
{
    _v.set(x, y, z, w);
}


//----------------------------------------
t3Quaternion::t3Quaternion(const t3Vector4f& v)
{
    _v = v;
}


//----------------------------------------
t3Quaternion::t3Quaternion(float angle, const t3Vector3f& axis)
{
    makeRotate(angle, axis);
}


//----------------------------------------
t3Quaternion::t3Quaternion(float angle1, const t3Vector3f& axis1, float angle2, const t3Vector3f& axis2, float angle3, const t3Vector3f& axis3)
{
    makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
}


//----------------------------------------
t3Quaternion& t3Quaternion::operator =(const t3Quaternion& q)
{
    _v = q._v;
    return *this;
}


//----------------------------------------
bool t3Quaternion::operator ==(const t3Quaternion& q) const
{
    return _v == q._v;
}


//----------------------------------------
bool t3Quaternion::operator !=(const t3Quaternion& q) const
{
    return _v != q._v;
}


//----------------------------------------
//bool t3Quaternion::operator <(const t3Quaternion& q) const {
//    if(_v.x < v._v.x) return true;
//    else if(_v.x > v._v.x) return false;
//    else if(_v.y < v._v.y) return true;
//    else if(_v.y > v._v.y) return false;
//    else if(_v.z < v._v.z) return true;
//    else if(_v.z > v._v.z) return false;
//    else return (_v.w < v._v.w);
//}



//----------------------------------------
t3Vector4f t3Quaternion::asVec4() const
{
    return _v;
}


//----------------------------------------
t3Vector3f t3Quaternion::asVec3() const
{
    return t3Vector3f(_v.x, _v.y, _v.z);
}


//----------------------------------------
void t3Quaternion::set(float x, float y, float z, float w)
{
    _v.set(x, y, z, w);
}


//----------------------------------------
void t3Quaternion::set(const t3Vector4f& v)
{
    _v = v;
}


//----------------------------------------
float& t3Quaternion::operator [](int i)
{
    return _v[i];
}



//----------------------------------------
float t3Quaternion::operator [](int i) const
{
    return _v[i];
}


//----------------------------------------
float& t3Quaternion::x()
{
    return _v.x;
}


//----------------------------------------
float& t3Quaternion::y()
{
    return _v.y;
}


//----------------------------------------
float& t3Quaternion::z()
{
    return _v.z;
}


//----------------------------------------
float& t3Quaternion::w()
{
    return _v.w;
}


//----------------------------------------
float t3Quaternion::x() const
{
    return _v.x;
}


//----------------------------------------
float t3Quaternion::y() const
{
    return _v.y;
}


//----------------------------------------
float t3Quaternion::z() const
{
    return _v.z;
}


//----------------------------------------
float t3Quaternion::w() const
{
    return _v.w;
}


//----------------------------------------
bool t3Quaternion::zeroRotation() const
{
    return _v.x == 0.0 && _v.y == 0.0 && _v.z == 0.0 && _v.w == 1.0;
}



//----------------------------------------
const t3Quaternion t3Quaternion::operator *(float rhs) const
{
    return t3Quaternion(_v.x*rhs, _v.y*rhs, _v.z*rhs, _v.w*rhs);
}


//----------------------------------------
t3Quaternion& t3Quaternion::operator *=(float rhs)
{
    _v.x *= rhs;
    _v.y *= rhs;
    _v.z *= rhs;
    _v.w *= rhs;
    return *this; // enable nesting
}


//----------------------------------------
const t3Quaternion t3Quaternion::operator*(const t3Quaternion& rhs) const
{
    return t3Quaternion(rhs._v.w*_v.x + rhs._v.x*_v.w + rhs._v.y*_v.z - rhs._v.z*_v.y,
                        rhs._v.w*_v.y - rhs._v.x*_v.z + rhs._v.y*_v.w + rhs._v.z*_v.x,
                        rhs._v.w*_v.z + rhs._v.x*_v.y - rhs._v.y*_v.x + rhs._v.z*_v.w,
                        rhs._v.w*_v.w - rhs._v.x*_v.x - rhs._v.y*_v.y - rhs._v.z*_v.z);
}


//----------------------------------------
t3Quaternion& t3Quaternion::operator*=(const t3Quaternion& rhs)
{
    float x = rhs._v.w * _v.x + rhs._v.x * _v.w + rhs._v.y * _v.z - rhs._v.z * _v.y;
    float y = rhs._v.w * _v.y - rhs._v.x * _v.z + rhs._v.y * _v.w + rhs._v.z * _v.x;
    float z = rhs._v.w * _v.z + rhs._v.x * _v.y - rhs._v.y * _v.x + rhs._v.z * _v.w;
    _v.w = rhs._v.w * _v.w - rhs._v.x * _v.x - rhs._v.y * _v.y - rhs._v.z * _v.z;

    _v.z = z;
    _v.y = y;
    _v.x = x;

    return (*this); // enable nesting
}


//----------------------------------------
t3Quaternion t3Quaternion::operator /(float rhs) const
{
    float div = 1.0f / rhs;
    return t3Quaternion(_v.x*div, _v.y*div, _v.z*div, _v.w*div);
}


//----------------------------------------
t3Quaternion& t3Quaternion::operator /=(float rhs)
{
    float div = 1.0f / rhs;
    _v.x *= div;
    _v.y *= div;
    _v.z *= div;
    _v.w *= div;
    return *this;
}


//----------------------------------------
const t3Quaternion t3Quaternion::operator/(const t3Quaternion& denom) const
{
    return ((*this) * denom.inverse());
}


//----------------------------------------
t3Quaternion& t3Quaternion::operator/=(const t3Quaternion& denom)
{
    (*this) = (*this) * denom.inverse();
    return (*this); // enable nesting
}


//----------------------------------------
const t3Quaternion t3Quaternion::operator +(const t3Quaternion& rhs) const
{
    return t3Quaternion(_v.x + rhs._v.x, _v.y + rhs._v.y,
                        _v.z + rhs._v.z, _v.w + rhs._v.w);
}


//----------------------------------------
t3Quaternion& t3Quaternion::operator +=(const t3Quaternion& rhs)
{
    _v.x += rhs._v.x;
    _v.y += rhs._v.y;
    _v.z += rhs._v.z;
    _v.w += rhs._v.w;
    return *this; // enable nesting
}


//----------------------------------------
const t3Quaternion t3Quaternion::operator -(const t3Quaternion& rhs) const
{
    return t3Quaternion(_v.x - rhs._v.x, _v.y - rhs._v.y,
                        _v.z - rhs._v.z, _v.w - rhs._v.w);
}


//----------------------------------------
t3Quaternion& t3Quaternion::operator -=(const t3Quaternion& rhs)
{
    _v.x -= rhs._v.x;
    _v.y -= rhs._v.y;
    _v.z -= rhs._v.z;
    _v.w -= rhs._v.w;
    return *this; // enable nesting
}


//----------------------------------------
const t3Quaternion t3Quaternion::operator -() const
{
    return t3Quaternion(-_v.x, -_v.y, -_v.z, -_v.w);
}


//----------------------------------------
float t3Quaternion::length() const
{
    return sqrt(_v.x*_v.x + _v.y*_v.y + _v.z*_v.z + _v.w*_v.w);
}


//----------------------------------------
float t3Quaternion::length2() const
{
    return _v.x*_v.x + _v.y*_v.y + _v.z*_v.z + _v.w*_v.w;
}


//----------------------------------------
t3Quaternion t3Quaternion::conj() const
{
    return t3Quaternion(-_v.x, -_v.y, -_v.z, _v.w);
}


//----------------------------------------
const t3Quaternion t3Quaternion::inverse() const
{
    return conj() / length2();
}



//----------------------------------------
t3Vector3f t3Quaternion::operator*(const t3Vector3f& v) const
{
    // nVidia SDK implementation
    t3Vector3f uv, uuv;
    t3Vector3f qvec(_v.x, _v.y, _v.z);
    uv = qvec.getCrossed(v);    //uv = qvec ^ v;
    uuv = qvec.getCrossed(uv);    //uuv = qvec ^ uv;
    uv *= (2.0f * _v.w);
    uuv *= 2.0f;
    return v + uv + uuv;
}

void t3Quaternion::normalize()
{
    float len = _v.w*_v.w + _v.x*_v.x + _v.y*_v.y + _v.z*_v.z;
    float factor = 1.0f / sqrt(len);
    _v.x *= factor;
    _v.y *= factor;
    _v.z *= factor;
    _v.w *= factor;
}


#endif