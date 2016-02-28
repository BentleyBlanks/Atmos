#ifndef T3_MATRIX4X4_INL
#define T3_MATRIX4X4_INL

#include <t3Math/core/t3Matri4x4.h>
#include <iostream>
#include <iomanip>

//--------------------------------------------------
// implementation of inline methods

inline bool t3Matrix4x4::isNaN() const
{

#if (_MSC_VER) || defined (T3_PLATFORM_ANDROID)
#ifndef isnan
#define isnan(a) ((a) != (a))
#endif

    return isnan(_mat[0][0]) || isnan(_mat[0][1]) || isnan(_mat[0][2]) || isnan(_mat[0][3]) ||
        isnan(_mat[1][0]) || isnan(_mat[1][1]) || isnan(_mat[1][2]) || isnan(_mat[1][3]) ||
        isnan(_mat[2][0]) || isnan(_mat[2][1]) || isnan(_mat[2][2]) || isnan(_mat[2][3]) ||
        isnan(_mat[3][0]) || isnan(_mat[3][1]) || isnan(_mat[3][2]) || isnan(_mat[3][3]);

#else
    return std::isnan(_mat[0][0]) || std::isnan(_mat[0][1]) || std::isnan(_mat[0][2]) || std::isnan(_mat[0][3]) ||
        std::isnan(_mat[1][0]) || std::isnan(_mat[1][1]) || std::isnan(_mat[1][2]) || std::isnan(_mat[1][3]) ||
        std::isnan(_mat[2][0]) || std::isnan(_mat[2][1]) || std::isnan(_mat[2][2]) || std::isnan(_mat[2][3]) ||
        std::isnan(_mat[3][0]) || std::isnan(_mat[3][1]) || std::isnan(_mat[3][2]) || std::isnan(_mat[3][3]);

#endif

}



inline std::ostream& operator<<(std::ostream& os, const t3Matrix4x4& M)
{
    int w = 8;
    os << std::setw(w)
        << M._mat[0][0] << ", " << std::setw(w)
        << M._mat[0][1] << ", " << std::setw(w)
        << M._mat[0][2] << ", " << std::setw(w)
        << M._mat[0][3] << std::endl;

    os << std::setw(w)
        << M._mat[1][0] << ", " << std::setw(w)
        << M._mat[1][1] << ", " << std::setw(w)
        << M._mat[1][2] << ", " << std::setw(w)
        << M._mat[1][3] << std::endl;

    os << std::setw(w)
        << M._mat[2][0] << ", " << std::setw(w)
        << M._mat[2][1] << ", " << std::setw(w)
        << M._mat[2][2] << ", " << std::setw(w)
        << M._mat[2][3] << std::endl;

    os << std::setw(w)
        << M._mat[3][0] << ", " << std::setw(w)
        << M._mat[3][1] << ", " << std::setw(w)
        << M._mat[3][2] << ", " << std::setw(w)
        << M._mat[3][3];

    return os;
}

inline std::istream& operator>>(std::istream& is, t3Matrix4x4& M)
{
    is >> M._mat[0][0]; is.ignore(2);
    is >> M._mat[0][1]; is.ignore(2);
    is >> M._mat[0][2]; is.ignore(2);
    is >> M._mat[0][3]; is.ignore(1);

    is >> M._mat[1][0]; is.ignore(2);
    is >> M._mat[1][1]; is.ignore(2);
    is >> M._mat[1][2]; is.ignore(2);
    is >> M._mat[1][3]; is.ignore(1);

    is >> M._mat[2][0]; is.ignore(2);
    is >> M._mat[2][1]; is.ignore(2);
    is >> M._mat[2][2]; is.ignore(2);
    is >> M._mat[2][3]; is.ignore(1);

    is >> M._mat[3][0]; is.ignore(2);
    is >> M._mat[3][1]; is.ignore(2);
    is >> M._mat[3][2]; is.ignore(2);
    is >> M._mat[3][3];
    return is;
}


inline t3Matrix4x4& t3Matrix4x4::operator = (const t3Matrix4x4& rhs)
{
    if(&rhs == this) return *this;
    set(rhs.getPtr());
    return *this;
}

inline void t3Matrix4x4::set(const t3Matrix4x4& rhs)
{
    set(rhs.getPtr());
}

inline void t3Matrix4x4::set(float const * const ptr)
{
    float* local_ptr = (float*) _mat;
    for(std::size_t i = 0; i < 16; ++i) local_ptr[i] = (float) ptr[i];
}

inline void t3Matrix4x4::set(double const * const ptr)
{
    float* local_ptr = (float*) _mat;
    for(std::size_t i = 0; i < 16; ++i) local_ptr[i] = (float) ptr[i];
}

inline bool t3Matrix4x4::isIdentity() const
{
    return _mat[0][0] == 1.0f && _mat[0][1] == 0.0f && _mat[0][2] == 0.0f &&  _mat[0][3] == 0.0f &&
        _mat[1][0] == 0.0f && _mat[1][1] == 1.0f && _mat[1][2] == 0.0f &&  _mat[1][3] == 0.0f &&
        _mat[2][0] == 0.0f && _mat[2][1] == 0.0f && _mat[2][2] == 1.0f &&  _mat[2][3] == 0.0f &&
        _mat[3][0] == 0.0f && _mat[3][1] == 0.0f && _mat[3][2] == 0.0f &&  _mat[3][3] == 1.0f;
}

inline void t3Matrix4x4::makeOrtho2DMatrix(double left, double right,
                                           double bottom, double top)
{
    makeOrthoMatrix(left, right, bottom, top, -1.0, 1.0);
}

inline t3Vector3f t3Matrix4x4::getTranslation() const
{
    return t3Vector3f(_mat[3][0], _mat[3][1], _mat[3][2]);
}

inline t3Vector3f t3Matrix4x4::getScale() const
{
    t3Vector3f x_vec(_mat[0][0], _mat[1][0], _mat[2][0]);
    t3Vector3f y_vec(_mat[0][1], _mat[1][1], _mat[2][1]);
    t3Vector3f z_vec(_mat[0][2], _mat[1][2], _mat[2][2]);
    return t3Vector3f(x_vec.length(), y_vec.length(), z_vec.length());
}

//static utility methods
inline t3Matrix4x4 t3Matrix4x4::newIdentityMatrix(void)
{
    t3Matrix4x4 m;
    m.makeIdentityMatrix();
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newScaleMatrix(float sx, float sy, float sz)
{
    t3Matrix4x4 m;
    m.makeScaleMatrix(sx, sy, sz);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newScaleMatrix(const t3Vector3f& v)
{
    return newScaleMatrix(v.x, v.y, v.z);
}

inline t3Matrix4x4 t3Matrix4x4::newTranslationMatrix(float tx, float ty, float tz)
{
    t3Matrix4x4 m;
    m.makeTranslationMatrix(tx, ty, tz);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newTranslationMatrix(const t3Vector3f& v)
{
    return newTranslationMatrix(v.x, v.y, v.z);
}

inline t3Matrix4x4 t3Matrix4x4::newRotationMatrix(const t3Quaternion& q)
{
    return t3Matrix4x4(q);
}
inline t3Matrix4x4 t3Matrix4x4::newRotationMatrix(float angle, float x, float y, float z)
{
    t3Matrix4x4 m;
    m.makeRotationMatrix(angle, x, y, z);
    return m;
}
inline t3Matrix4x4 t3Matrix4x4::newRotationMatrix(float angle, const t3Vector3f& axis)
{
    t3Matrix4x4 m;
    m.makeRotationMatrix(angle, axis);
    return m;
}
inline t3Matrix4x4 t3Matrix4x4::newRotationMatrix(float angle1, const t3Vector3f& axis1,
                                                  float angle2, const t3Vector3f& axis2,
                                                  float angle3, const t3Vector3f& axis3)
{
    t3Matrix4x4 m;
    m.makeRotationMatrix(angle1, axis1, angle2, axis2, angle3, axis3);
    return m;
}
inline t3Matrix4x4 t3Matrix4x4::newRotationMatrix(const t3Vector3f& from, const t3Vector3f& to)
{
    t3Matrix4x4 m;
    m.makeRotationMatrix(from, to);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::getInverseOf(const t3Matrix4x4& matrix)
{
    t3Matrix4x4 m;
    m.makeInvertOf(matrix);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::getTransposedOf(const t3Matrix4x4& matrix)
{
    t3Matrix4x4 m(matrix._mat[0][0], matrix._mat[1][0], matrix._mat[2][0],
                  matrix._mat[3][0], matrix._mat[0][1], matrix._mat[1][1], matrix._mat[2][1],
                  matrix._mat[3][1], matrix._mat[0][2], matrix._mat[1][2], matrix._mat[2][2],
                  matrix._mat[3][2], matrix._mat[0][3], matrix._mat[1][3], matrix._mat[2][3],
                  matrix._mat[3][3]);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::getOrthoNormalOf(const t3Matrix4x4& matrix)
{
    t3Matrix4x4 m;
    m.makeOrthoNormalOf(matrix);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newOrthoMatrix(double left, double right,
                                               double bottom, double top,
                                               double zNear, double zFar)
{
    t3Matrix4x4 m;
    m.makeOrthoMatrix(left, right, bottom, top, zNear, zFar);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newOrtho2DMatrix(double left, double right,
                                                 double bottom, double top)
{
    t3Matrix4x4 m;
    m.makeOrtho2DMatrix(left, right, bottom, top);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newFrustumMatrix(double left, double right,
                                                 double bottom, double top,
                                                 double zNear, double zFar)
{
    t3Matrix4x4 m;
    m.makeFrustumMatrix(left, right, bottom, top, zNear, zFar);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newPerspectiveMatrix(double fovy, double aspectRatio,
                                                     double zNear, double zFar)
{
    t3Matrix4x4 m;
    m.makePerspectiveMatrix(fovy, aspectRatio, zNear, zFar);
    return m;
}

inline t3Matrix4x4 t3Matrix4x4::newLookAtMatrix(const t3Vector3f& eye, const t3Vector3f& center, const t3Vector3f& up)
{
    t3Matrix4x4 m;
    m.makeLookAtMatrix(eye, center, up);
    return m;
}

inline t3Vector3f t3Matrix4x4::postMult(const t3Vector3f& v) const
{
    float d = 1.0f / (_mat[3][0] * v.x + _mat[3][1] * v.y + _mat[3][2] * v.z + _mat[3][3]);
    return t3Vector3f((_mat[0][0] * v.x + _mat[0][1] * v.y + _mat[0][2] * v.z + _mat[0][3])*d,
                      (_mat[1][0] * v.x + _mat[1][1] * v.y + _mat[1][2] * v.z + _mat[1][3])*d,
                      (_mat[2][0] * v.x + _mat[2][1] * v.y + _mat[2][2] * v.z + _mat[2][3])*d);
}

inline t3Vector3f t3Matrix4x4::preMult(const t3Vector3f& v) const
{
    float d = 1.0f / (_mat[0][3] * v.x + _mat[1][3] * v.y + _mat[2][3] * v.z + _mat[3][3]);
    return t3Vector3f((_mat[0][0] * v.x + _mat[1][0] * v.y + _mat[2][0] * v.z + _mat[3][0])*d,
                      (_mat[0][1] * v.x + _mat[1][1] * v.y + _mat[2][1] * v.z + _mat[3][1])*d,
                      (_mat[0][2] * v.x + _mat[1][2] * v.y + _mat[2][2] * v.z + _mat[3][2])*d);
}

/// \brief post-multiplies the vector by the matrix (i.e. returns M mult v).
/// The vector is implicitly treated as a column-matrix
inline t3Vector4f t3Matrix4x4::postMult(const t3Vector4f& v) const
{
    return t3Vector4f((_mat[0][0] * v.x + _mat[0][1] * v.y + _mat[0][2] * v.z + _mat[0][3] * v.w),
                      (_mat[1][0] * v.x + _mat[1][1] * v.y + _mat[1][2] * v.z + _mat[1][3] * v.w),
                      (_mat[2][0] * v.x + _mat[2][1] * v.y + _mat[2][2] * v.z + _mat[2][3] * v.w),
                      (_mat[3][0] * v.x + _mat[3][1] * v.y + _mat[3][2] * v.z + _mat[3][3] * v.w));
}

/// \brief pre-multiplies the vector by the matrix (i.e. returns v mult M)
/// The vector is implicitly treated as a row-matrix
inline t3Vector4f t3Matrix4x4::preMult(const t3Vector4f& v) const
{
    return t3Vector4f((_mat[0][0] * v.x + _mat[1][0] * v.y + _mat[2][0] * v.z + _mat[3][0] * v.w),
                      (_mat[0][1] * v.x + _mat[1][1] * v.y + _mat[2][1] * v.z + _mat[3][1] * v.w),
                      (_mat[0][2] * v.x + _mat[1][2] * v.y + _mat[2][2] * v.z + _mat[3][2] * v.w),
                      (_mat[0][3] * v.x + _mat[1][3] * v.y + _mat[2][3] * v.z + _mat[3][3] * v.w));
}

/// \brief performs a pre-multiplication transformation on the vector using only the
/// upper left 3x3 portion of the matrix (i.e. only the rotation part).
inline t3Vector3f t3Matrix4x4::transform3x3(const t3Vector3f& v, const t3Matrix4x4& m)
{
    return t3Vector3f((m._mat[0][0] * v.x + m._mat[1][0] * v.y + m._mat[2][0] * v.z),
                      (m._mat[0][1] * v.x + m._mat[1][1] * v.y + m._mat[2][1] * v.z),
                      (m._mat[0][2] * v.x + m._mat[1][2] * v.y + m._mat[2][2] * v.z));
}

/// \brief performs a post-multiplication transformation on the vector using only the
/// upper left 3x3 portion of the matrix (i.e. only the rotation part).
inline t3Vector3f t3Matrix4x4::transform3x3(const t3Matrix4x4& m, const t3Vector3f& v)
{
    return t3Vector3f((m._mat[0][0] * v.x + m._mat[0][1] * v.y + m._mat[0][2] * v.z),
                      (m._mat[1][0] * v.x + m._mat[1][1] * v.y + m._mat[1][2] * v.z),
                      (m._mat[2][0] * v.x + m._mat[2][1] * v.y + m._mat[2][2] * v.z));
}

/// \brief translates this matrix by treating the t3Vector3f like a translation matrix,
/// and multiplying this Matrix by it in a pre-multiplication manner (T mult M)
inline void t3Matrix4x4::preMultTranslate(const t3Vector3f& v)
{
    for(unsigned i = 0; i < 3; ++i)
    {
        float tmp = v.getPtr()[i];
        if(tmp == 0)
            continue;
        _mat[3][0] += tmp * _mat[i][0];
        _mat[3][1] += tmp * _mat[i][1];
        _mat[3][2] += tmp * _mat[i][2];
        _mat[3][3] += tmp * _mat[i][3];
    }
}

/// \brief translates this matrix by treating the t3Vector3f like a translation matrix,
/// and multiplying this Matrix by it in a post-multiplication manner (M mult T)
inline void t3Matrix4x4::postMultTranslate(const t3Vector3f& v)
{
    for(std::size_t i = 0; i < 3; ++i)
    {
        float tmp = v.getPtr()[i];
        if(tmp == 0)
            continue;
        _mat[0][i] += tmp * _mat[0][3];
        _mat[1][i] += tmp * _mat[1][3];
        _mat[2][i] += tmp * _mat[2][3];
        _mat[3][i] += tmp * _mat[3][3];
    }
}

// AARON METHOD
/// \brief the positional argument version of the above
inline void t3Matrix4x4::postMultTranslate(float x, float y, float z)
{
    if(x != 0)
    {
        _mat[0][0] += x * _mat[0][3];
        _mat[1][0] += x * _mat[1][3];
        _mat[2][0] += x * _mat[2][3];
        _mat[3][0] += x * _mat[3][3];
    }
    if(y != 0)
    {
        _mat[0][1] += y * _mat[0][3];
        _mat[1][1] += y * _mat[1][3];
        _mat[2][1] += y * _mat[2][3];
        _mat[3][1] += y * _mat[3][3];
    }
    if(z != 0)
    {
        _mat[0][2] += z * _mat[0][3];
        _mat[1][2] += z * _mat[1][3];
        _mat[2][2] += z * _mat[2][3];
        _mat[3][2] += z * _mat[3][3];
    }
}

/// \brief treats the t3Vector3f like a scaling matrix and edits this Matrix
/// by multiplying the vector with it in a pre-multiplication style (V mult M)
inline void t3Matrix4x4::preMultScale(const t3Vector3f& v)
{
    _mat[0][0] *= v.getPtr()[0];
    _mat[0][1] *= v.getPtr()[0];
    _mat[0][2] *= v.getPtr()[0];
    _mat[0][3] *= v.getPtr()[0];
    _mat[1][0] *= v.getPtr()[1];
    _mat[1][1] *= v.getPtr()[1];
    _mat[1][2] *= v.getPtr()[1];
    _mat[1][3] *= v.getPtr()[1];
    _mat[2][0] *= v.getPtr()[2];
    _mat[2][1] *= v.getPtr()[2];
    _mat[2][2] *= v.getPtr()[2];
    _mat[2][3] *= v.getPtr()[2];
}

/// \brief treats the t3Vector3f like a scaling matrix and edits this Matrix
/// by multiplying the vector with it in a post-multiplication style (M mult V)
inline void t3Matrix4x4::postMultScale(const t3Vector3f& v)
{
    _mat[0][0] *= v.getPtr()[0];
    _mat[1][0] *= v.getPtr()[0];
    _mat[2][0] *= v.getPtr()[0];
    _mat[3][0] *= v.getPtr()[0];
    _mat[0][1] *= v.getPtr()[1];
    _mat[1][1] *= v.getPtr()[1];
    _mat[2][1] *= v.getPtr()[1];
    _mat[3][1] *= v.getPtr()[1];
    _mat[0][2] *= v.getPtr()[2];
    _mat[1][2] *= v.getPtr()[2];
    _mat[2][2] *= v.getPtr()[2];
    _mat[3][2] *= v.getPtr()[2];
}

/// \brief rotates this Matrix by the provided quaternion
inline void t3Matrix4x4::rotate(const t3Quaternion& q)
{
    postMultRotate(q);
}

/// \brief rotates this Matrix by the provided angle (in degrees) around an axis defined by the three values
inline void t3Matrix4x4::rotate(float angle, float x, float y, float z)
{
    postMultRotate(angle, x, y, z);
}

/// \brief Rotates this Matrix by the provided angle (in Radians) around an axis defined by the three values
inline void t3Matrix4x4::rotateRad(float angle, float x, float y, float z)
{
    postMultRotate(angle*static_cast<float>(T3MATH_RAD_TO_DEG), x, y, z);
}

/// \brief Translates this matrix by the provided amount
inline void t3Matrix4x4::translate(float tx, float ty, float tz)
{
    postMultTranslate(tx, ty, tz);
}

/// \brief Translates this matrix by the provided vector
inline void t3Matrix4x4::translate(const t3Vector3f& v)
{
    postMultTranslate(v);
}

/// \brief scales this matrix by the provided scales
inline void t3Matrix4x4::scale(float x, float y, float z)
{
    postMultScale(x, y, z);
}

/// \brief scales this matrix, treating the t3Vector3f as the diagonal of a scaling matrix.
inline void t3Matrix4x4::scale(const t3Vector3f& v)
{
    postMultScale(v);
}

/// implementation of the gl-style pre-multiplication versions of the above functions
inline void t3Matrix4x4::glRotate(float angle, float x, float y, float z)
{
    preMultRotate(t3Quaternion(angle, t3Vector3f(x, y, z)));
}

inline void t3Matrix4x4::glRotateRad(float angle, float x, float y, float z)
{
    preMultRotate(t3Quaternion(angle*static_cast<float>(T3MATH_RAD_TO_DEG), t3Vector3f(x, y, z)));
}

inline void t3Matrix4x4::glRotate(const t3Quaternion& q)
{
    preMultRotate(q);
}

inline void t3Matrix4x4::glTranslate(float tx, float ty, float tz)
{
    preMultTranslate(t3Vector3f(tx, ty, tz));
}

inline void t3Matrix4x4::glTranslate(const t3Vector3f& v)
{
    preMultTranslate(v);
}

inline void t3Matrix4x4::glScale(float x, float y, float z)
{
    preMultScale(t3Vector3f(x, y, z));
}

inline void t3Matrix4x4::glScale(const t3Vector3f& v)
{
    preMultScale(v);
}

// AARON METHOD
inline void t3Matrix4x4::postMultScale(float x, float y, float z)
{
    _mat[0][0] *= x;
    _mat[1][0] *= x;
    _mat[2][0] *= x;
    _mat[3][0] *= x;
    _mat[0][1] *= y;
    _mat[1][1] *= y;
    _mat[2][1] *= y;
    _mat[3][1] *= y;
    _mat[0][2] *= z;
    _mat[1][2] *= z;
    _mat[2][2] *= z;
    _mat[3][2] *= z;
}


inline void t3Matrix4x4::preMultRotate(const t3Quaternion& q)
{
    if(q.zeroRotation())
        return;
    t3Matrix4x4 r;
    r.setRotate(q);
    preMult(r);
}

inline void t3Matrix4x4::postMultRotate(const t3Quaternion& q)
{
    if(q.zeroRotation())
        return;
    t3Matrix4x4 r;
    r.setRotate(q);
    postMult(r);
}

// AARON METHOD
inline void t3Matrix4x4::postMultRotate(float angle, float x, float y, float z)
{
    t3Matrix4x4 r;
    r.makeRotationMatrix(angle, x, y, z);
    postMult(r);
}

/// \brief provides Vector3 * Matrix multiplication. Vectors are implicitly treated as row-matrices.
inline t3Vector3f operator* (const t3Vector3f& v, const t3Matrix4x4& m)
{
    return m.preMult(v);
}
/// \brief provides Vector4 * Matrix multiplication. Vectors are implicitly treated as row-matrices.
inline t3Vector4f operator* (const t3Vector4f& v, const t3Matrix4x4& m)
{
    return m.preMult(v);
}

#endif