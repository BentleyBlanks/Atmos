#ifndef T3_MATH_INL
#define T3_MATH_INL

#include <t3Math/core/t3Math.h>

#include <assert.h> 
#include <math.h>

T3_FORCE_INLINE float t3Math::e()
{
    return T3MATH_E;
}

T3_FORCE_INLINE float t3Math::pi()
{
    return T3MATH_PI;
}

T3_FORCE_INLINE float t3Math::Deg2Rad(float f)
{
    return f * T3MATH_DEG_TO_RAD;
}

T3_FORCE_INLINE float t3Math::Rad2Deg(float f)
{
    return f * T3MATH_RAD_TO_DEG;
}

T3_FORCE_INLINE float t3Math::sqrt(float f)
{
    return ::sqrtf(f);
}

T3_FORCE_INLINE double t3Math::sqrtd(double f)
{
    return ::sqrt(f);
}

T3_FORCE_INLINE float t3Math::sqrtInverse(float f)
{
    return (1.0f / sqrtf(f));
}

T3_FORCE_INLINE float t3Math::Abs(float t)
{
    return (t < 0 ? -t : t);
}

T3_FORCE_INLINE double t3Math::Abs(double t)
{
    return (t < 0 ? -t : t);
}

T3_FORCE_INLINE int t3Math::Abs(int t)
{
    return (t < 0 ? -t : t);
}

T3_FORCE_INLINE float t3Math::exp(float exponent)
{
    return ::expf(exponent);
}

T3_FORCE_INLINE float t3Math::pow(float base, float exponent)
{
    return ::powf(base, exponent);
}

template<class T>
T3_FORCE_INLINE T t3Math::square(T value)
{
    return (value * value);
}

T3_FORCE_INLINE float t3Math::pow2(float exponent)
{
    return pow(2.0f, exponent);
}

T3_FORCE_INLINE unsigned int t3Math::pow2(unsigned int exponent)
{
    return (1U << exponent);
}

T3_FORCE_INLINE float t3Math::ceil(float f)
{
    return ::ceilf(f);
}

T3_FORCE_INLINE float t3Math::floor(float f)
{
    return ::floorf(f);
}

T3_FORCE_INLINE float t3Math::sinDeg(float deg)
{
    return ::sinf(deg * T3MATH_DEG_TO_RAD);
}

T3_FORCE_INLINE float t3Math::sinRad(float rad)
{
    return ::sinf(rad);
}

T3_FORCE_INLINE float t3Math::cosDeg(float deg)
{
    return ::cosf(deg * T3MATH_DEG_TO_RAD);
}

T3_FORCE_INLINE float t3Math::cosRad(float rad)
{
    return ::cosf(rad);
}

T3_FORCE_INLINE float t3Math::tanDeg(float deg)
{
    return ::tan(deg * T3MATH_DEG_TO_RAD);
}

T3_FORCE_INLINE float t3Math::tanRad(float rad)
{
    return ::tan(rad);
}

T3_FORCE_INLINE float t3Math::asinDeg(float f)
{
    return ::asinf(f) * T3MATH_RAD_TO_DEG;
}

T3_FORCE_INLINE float t3Math::asinRad(float f)
{
    return ::asinf(f);
}

T3_FORCE_INLINE float t3Math::acosDeg(float f)
{
    return ::acosf(f) * T3MATH_RAD_TO_DEG;
}

T3_FORCE_INLINE float t3Math::acosRad(float f)
{
    return ::acosf(f);
}

T3_FORCE_INLINE float t3Math::atanDeg(float f)
{
    return ::atan(f) * T3MATH_RAD_TO_DEG;
}

T3_FORCE_INLINE float t3Math::atanRad(float f)
{
    return ::atan(f);
}

T3_FORCE_INLINE float t3Math::atan2Deg(float y, float x)
{
    return ::atan2f(y, x) * T3MATH_RAD_TO_DEG;
}

T3_FORCE_INLINE float t3Math::atan2Rad(float y, float x)
{
    return ::atan2f(y, x);
}

T3_FORCE_INLINE bool t3Math::isAngleEqualDeg(float fAngle1, float fAngle2, float fEpsilon /* = t3Math_DEFAULT_EPSILON */)
{
    assert(fEpsilon >= 0.0f);

    while(fAngle1 < 0.0f)
        fAngle1 += 360.0f;
    while(fAngle2 < 0.0f)
        fAngle2 += 360.0f;

    while(fAngle1 >= 360.0f)
        fAngle1 -= 360.0f;
    while(fAngle2 >= 360.0f)
        fAngle2 -= 360.0f;

    return isFloatEqual(fAngle1, fAngle2, fEpsilon);
}

T3_FORCE_INLINE float t3Math::log(float f)
{
    return ::logf(f);
}

T3_FORCE_INLINE float t3Math::log2(float f)
{
    return log10(f) / log10(2.0f);
}

T3_FORCE_INLINE float t3Math::log10(float f)
{
    return ::log10f(f);
}

T3_FORCE_INLINE float t3Math::logB(float f, float base)
{
    return log10(f) / log10(base);
}

T3_FORCE_INLINE bool t3Math::isFloatEqual(float f1, float f2, float fEpsilon)
{
    // epsilon MUST be larger than zero, otherwise hkMath::equal will fail
    assert(fEpsilon >= 0.0f);

    if(fEpsilon == 0.0f)
        return (f1 == f2);

    // phrasing the test this way will make sure that NaN's will also return 'false'
    return ((f1 >= f2 - fEpsilon) && (f1 <= f2 + fEpsilon));
}

T3_FORCE_INLINE bool t3Math::isFloatEqual(double f1, double f2, double fEpsilon)
{
    // epsilon MUST be larger than zero, otherwise hkMath::equal will fail
    assert(fEpsilon >= 0.0);

    if(fEpsilon == 0.0)
        return (f1 == f2);

    // phrasing the test this way will make sure that NaN's will also return 'false'
    return ((f1 >= f2 - fEpsilon) && (f1 <= f2 + fEpsilon));
}

T3_FORCE_INLINE bool t3Math::isZero(float f, float fEpsilon)
{
    return isFloatEqual(f, 0.0f, fEpsilon);
}

T3_FORCE_INLINE bool t3Math::isZero(double f, double fEpsilon)
{
    return isFloatEqual(f, 0.0, fEpsilon);
}


template<class T>
T3_FORCE_INLINE T t3Math::Min(T t1, T t2)
{
    return (t1 < t2 ? t1 : t2);
}

template<class T>
T3_FORCE_INLINE T t3Math::Min(T t1, T t2, T t3)
{
    return t3Math::Min<T>(t3Math::Min<T>(t1, t2), t3);
}

template<class T>
T3_FORCE_INLINE T t3Math::Min(T t1, T t2, T t3, T t4)
{
    return t3Math::Min<T>(t3Math::Min<T>(t1, t2), t3Math::Min<T>(t3, t4));
}

template<class T>
T3_FORCE_INLINE T t3Math::Min(T t1, T t2, T t3, T t4, T t5)
{
    return t3Math::Min<T>(t3Math::Min<T>(t1, t2, t3, t4), t5);
}

template<class T>
T3_FORCE_INLINE T t3Math::Max(T t1, T t2)
{
    return (t1 < t2 ? t2 : t1);
}

template<class T>
T3_FORCE_INLINE T t3Math::Max(T t1, T t2, T t3)
{
    return t3Math::Max<T>(t3Math::Max<T>(t1, t2), t3);
}

template<class T>
T3_FORCE_INLINE T t3Math::Max(T t1, T t2, T t3, T t4)
{
    return t3Math::Max<T>(t3Math::Max<T>(t1, t2), t3Math::Max<T>(t3, t4));
}

template<class T>
T3_FORCE_INLINE T t3Math::Max(T t1, T t2, T t3, T t4, T t5)
{
    return t3Math::Max<T>(t3Math::Max<T>(t1, t2, t3, t4), t5);
}

#if defined( T2_PLATFORM_XENON ) || ( defined( T2_PLATFORM_XENON ) && !defined( SPU ) )

template<>
T3_FORCE_INLINE float t3Math::Min<float>(float t1, float t2)
{
    return (float) __fsel(t1 - t2, t2, t1);
}

template<>
T3_FORCE_INLINE double t3Math::Min<double>(double t1, double t2)
{
    return __fsel(t1 - t2, t2, t1);
}

template<>
T3_FORCE_INLINE float t3Math::Max<float>(float t1, float t2)
{
    return (float) __fsel(t1 - t2, t1, t2);
}

template<>
T3_FORCE_INLINE double t3Math::Max<double>(double t1, double t2)
{
    return __fsel(t1 - t2, t1, t2);
}

#endif

template<class T>
T3_FORCE_INLINE T t3Math::clamp(T val, T tMin, T tMax)
{
    assert(tMin <= tMax);

    if(val < tMin)
        return tMin;
    if(val > tMax)
        return tMax;

    return val;
}

template<class T>
T3_FORCE_INLINE T t3Math::saturate(T val)
{
    return clamp(val, T(0), T(1));
}

T3_FORCE_INLINE float t3Math::mod(float f, float d)
{
    return f - (d * float2int(f / d));
}

T3_FORCE_INLINE bool t3Math::isPowerOf(unsigned int x, unsigned int base)
{
    assert(base > 1);

    if(x == 1)
        return true;

    while(x > base)
    {
        if(x % base == 0)
            x /= base;
        else
            return (false);
    }

    return (x == base);
}

T3_FORCE_INLINE bool t3Math::isPowerOf2(unsigned int x)
{
    return ((x != 0) && ((x & (x - 1U)) == 0));
}

template<class T>
T3_FORCE_INLINE T t3Math::interpolate(const T& val1, const T& val2, float fInterpolation)
{
    return val1 * (1.0f - fInterpolation) + val2 * fInterpolation;
}

template<class T>
T3_FORCE_INLINE T t3Math::interpolate(const T& val1, const T& val2, double fInterpolation)
{
    return val1 * (1.0 - fInterpolation) + val2 * fInterpolation;
}


T3_FORCE_INLINE float t3Math::root(float f, float fNthRoot)
{
    assert((f >= 0.0f) && (fNthRoot != 0.0f));

    return pow(f, 1.0f / fNthRoot);
}

template<class T>
T3_FORCE_INLINE T t3Math::sign(T t)
{
    if(t == 0)
        return 0;

    return t < 0 ? (T) (-1) : (T) (1);
}

T3_FORCE_INLINE float t3Math::round(float f)
{
    return floor(f + 0.5f);
}

T3_FORCE_INLINE float t3Math::roundTo(float f, float fRoundTo)
{
    return (round(f / fRoundTo) * fRoundTo);
}

T3_FORCE_INLINE int t3Math::float2int(float f)
{
    return (int) f;
}

T3_FORCE_INLINE float t3Math::fraction(float f)
{
    return f - float2int(f);
}

template<class T>
T3_FORCE_INLINE void t3Math::swap(T& v1, T& v2)
{
    T temp = v1;
    v1 = v2;
    v2 = temp;
}

T3_FORCE_INLINE unsigned int t3Math::powerOf2_floor(unsigned int x)
{
    return powerOf2_ceil(x / 2 + 1);
}

T3_FORCE_INLINE unsigned int t3Math::powerOf2_ceil(unsigned int x)
{
    // http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2

    // The method below returns 0 for x == 0, so fix this case manually
    x += (x == 0);

    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;

    return x;
}

T3_FORCE_INLINE float t3Math::generateNaN(void)
{
    union
    {
        float f;
        unsigned int i;
    } val;

    // Usually NAN == (exponent = all 1, mantissa = non-zero)
    //         INF == (exponent = all 1, mantissa = zero)

    // = 0111 1111 1000 0000 0000 0000 0000 0001
    val.i = 0x7f800001;

    return val.f;
}

T3_FORCE_INLINE float t3Math::generateInfinity(void)
{
    union
    {
        float f;
        unsigned int i;
    } val;

    // Usually NAN == (exponent = all 1, mantissa = non-zero)
    //         INF == (exponent = all 1, mantissa = zero)

    // = 0111 1111 1000 0000 0000 0000 0000 0000
    val.i = 0x7f800000;

    return val.f;
}

T3_FORCE_INLINE bool t3Math::isNaN(float r)
{
    // Check the 8 exponent bits.
    // Usually NAN == (exponent = all 1, mantissa = non-zero)
    //         INF == (exponent = all 1, mantissa = zero)
    // This simply checks the exponent

    union
    {
        float f;
        unsigned int i;
    } val;

    val.f = r;
    return (((val.i & 0x7f800000) == 0x7f800000) && ((val.i & 0x7FFFFF) != 0));


    // all comparisons fail, if a float is NaN
    // even the equality test fails with a NaN
    // for all valid numbers 'f == f' will succeed and thus the final result will be false
    // for NaN 'f == f' will fail and thus the final result will be true

    // Note: DO NOT change it into 'f != f' !!!

    // Note: This code does not run in 64 Bit (Release), probably because the compiler optimizes too much away.
    //return (! (f == f));
}

T3_FORCE_INLINE bool t3Math::isFiniteNumber(float r)
{
    // Check the 8 exponent bits.
    // Usually NAN == (exponent = all 1, mantissa = non-zero)
    //         INF == (exponent = all 1, mantissa = zero)
    // This simply checks the exponent

    union
    {
        float f;
        unsigned int i;
    } val;

    val.f = r;
    return ((val.i & 0x7f800000) != 0x7f800000);


    // If f is inifinity or -infinity one of these comparisions will fail
    // If f is NaN, both comparisions will fail

    // Thus this function will only return true for a valid, finite number

    //return ((f <= t3Math_FLOAT_MAX_POS) && (f >= t3Math_FLOAT_MAX_NEG));
}

template<class T>
T3_FORCE_INLINE bool t3Math::isInRange(T val, T minVal, T maxVal)
{
    if(minVal < maxVal)
        return (val >= minVal) && (val <= maxVal);
    else
        return (val <= minVal) && (val >= maxVal);
}


#endif