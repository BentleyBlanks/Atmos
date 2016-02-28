#ifndef T3_MATH_H
#define T3_MATH_H

#include <t3Math/core/t3MathSettings.h>

// --!http://anarchy.cn/manual/11/VisionAPI/classhkvMath.html 
class t3Math
{
public:
    // -----------------------Constants and Conversions-----------------------
    // Returns Euler's number (2.718...). 
    static T3_FORCE_INLINE float e();

    // Returns Pi (3.141...). 
    static T3_FORCE_INLINE float pi();

    // Returns the largest positive value that a 32 Bit float can represent. 
    //static T3_FORCE_INLINE float FloatMaxPos();

    // Returns the largest negative value that a 32 Bit float can represent. 
    //static T3_FORCE_INLINE float FloatMaxNeg();

    // -----------------------Trigonometric Functions-----------------------
    // Converts a given value from degree to radians. 
    static T3_FORCE_INLINE float Deg2Rad(float f);

    // Converts a given value from radians to degree. 
    static T3_FORCE_INLINE float Rad2Deg(float f);

    // Returns the sine of the given value, which is expected to be in degree. 
    static T3_FORCE_INLINE float sinDeg(float deg);

    // Returns the sine of the given value, which is expected to be in radians. 
    static T3_FORCE_INLINE float sinRad(float rad);

    // Returns the cosine of the given value, which is expected to be in degree. 
    static T3_FORCE_INLINE float cosDeg(float deg);

    // Returns the cosine of the given value, which is expected to be in radians. 
    static T3_FORCE_INLINE float cosRad(float rad);

    // Returns the tangents of the given value, which is expected to be in degree. 
    static T3_FORCE_INLINE float tanDeg(float deg);

    // Returns the tangents of the given value, which is expected to be in radians. 
    static T3_FORCE_INLINE float tanRad(float rad);

    // Returns the inverse sine (in degree) of f. 
    static T3_FORCE_INLINE float asinDeg(float f);

    // Returns the inverse sine (in radians) of f. 
    static T3_FORCE_INLINE float asinRad(float f);

    // Returns the inverse cosine (in degree) of f. 
    static T3_FORCE_INLINE float acosDeg(float f);

    // Returns the inverse cosine (in radians) of f. 
    static T3_FORCE_INLINE float acosRad(float f);

    // Returns the inverse tangents (in degree) of f. 
    static T3_FORCE_INLINE float atanDeg(float f);

    // Returns the inverse tangents (in radians) of f. 
    static T3_FORCE_INLINE float atanRad(float f);

    // Returns the inverse tangents (in degree) of (y / x). 
    static T3_FORCE_INLINE float atan2Deg(float y, float x);

    // Returns the inverse tangents (in radians) of (y / x). 
    static T3_FORCE_INLINE float atan2Rad(float y, float x);

    // Checks whether two angles, given in degree, are equal. Wraps around 360, if necessary. 
    static T3_FORCE_INLINE bool isAngleEqualDeg(float fAngle1, float fAngle2, float fEpsilon = T3MATH_DEFAULT_EPSILON);

    // -----------------------Exponential Functions-----------------------
    // Returns the square root of f. 
    static T3_FORCE_INLINE float sqrt(float f);

    // Returns the square root of f. 
    static T3_FORCE_INLINE double sqrtd(double f);

    // Returns the inverse square root of f, ie. (1.0f / sqrt (f)). 
    static T3_FORCE_INLINE float sqrtInverse(float f);

    // Returns the n-th root of f. 
    static T3_FORCE_INLINE float root(float f, float fNthRoot);

    // Returns 'pow (e, exponent)' with e being Euler's number. 
    static T3_FORCE_INLINE float exp(float exponent);

    // Returns 'base ^ exponent'. 
    static T3_FORCE_INLINE float pow(float base, float exponent);

    // Returns 'value * value'.
    template<class T >
    static T3_FORCE_INLINE T square(T value);

    // Returns '2.0f ^ exponent'.
    static T3_FORCE_INLINE float pow2(float exponent);

    // Returns '2 ^ exponent'. Implemented as a left shift. 
    static T3_FORCE_INLINE unsigned int pow2(unsigned int exponent);

    // Checks whether x is base raised to any power. 
    static T3_FORCE_INLINE bool isPowerOf(unsigned int x, unsigned int base);

    // Checks whether x is 2 raised to any power. 
    static T3_FORCE_INLINE bool isPowerOf2(unsigned int x);

    // Returns the logarithm of f using e as its base. 
    static T3_FORCE_INLINE float log(float f);

    // Returns the logarithm of f using 2 as its base. 
    static T3_FORCE_INLINE float log2(float f);

    // Returns the logarithm of f using 10 as its base. 
    static T3_FORCE_INLINE float log10(float f);

    // Returns the logarithm of f using 'base' as its base. 
    static T3_FORCE_INLINE float logB(float f, float base);

    // -----------------------Clamping and Rounding-----------------------
    // Returns the sign(0, -1 or + 1) of t.
    template<class T >
    static T3_FORCE_INLINE T sign(T t);

    // Returns the absolute value of t. 
    static T3_FORCE_INLINE int Abs(int t);

    // Returns the absolute value of t. 
    static T3_FORCE_INLINE float Abs(float t);

    // Returns the absolute value of t. 
    static T3_FORCE_INLINE double Abs(double t);

    // Returns the smallest integral value that is not less than f. 
    static T3_FORCE_INLINE float ceil(float f);

    // Returns the largest integral value that is not greater than f. 
    static T3_FORCE_INLINE float floor(float f);

    // Returns the smallest power of two that is larger or equal to x. 
    static T3_FORCE_INLINE unsigned int powerOf2_ceil(unsigned int x);

    // Returns the largest power of two that is smaller or equal to x. 
    static T3_FORCE_INLINE unsigned int powerOf2_floor(unsigned int x);

    // Rounds f to the closest integral value. 
    static T3_FORCE_INLINE float round(float f);

    // Rounds f to the closest multiple of fRoundTo. 
    static T3_FORCE_INLINE float roundTo(float f, float fRoundTo);

    // Returns the integral part of f as an int. 
    static T3_FORCE_INLINE int float2int(float f);

    // Returns the fractional part of f. 
    static T3_FORCE_INLINE float fraction(float f);

    // Returns the smaller value t1 or t2. 
    template<class T >
    static T3_FORCE_INLINE T Min(T t1, T t2);

    // Returns the smallest value t1 or t2 or t3. 
    template<class T >
    static T3_FORCE_INLINE T Min(T t1, T t2, T t3);

    // Returns the smallest value t1 or t2 or t3 or t4. 
    template<class T >
    static T3_FORCE_INLINE T Min(T t1, T t2, T t3, T t4);

    // Returns the smallest value t1 or t2 or t3 or t4 or t5. 
    template<class T >
    static T3_FORCE_INLINE T Min(T t1, T t2, T t3, T t4, T t5);

    // Returns the larger value t1 or t2. 
    template<class T >
    static T3_FORCE_INLINE T Max(T t1, T t2);

    // Returns the largest value t1 or t2 or t3. 
    template<class T >
    static T3_FORCE_INLINE T Max(T t1, T t2, T t3);

    // Returns the largest value t1 or t2 or t3 or t4. 
    template<class T >
    static T3_FORCE_INLINE T Max(T t1, T t2, T t3, T t4);

    // Returns the largest value t1 or t2 or t3 or t4 or t5. 
    template<class T >
    static T3_FORCE_INLINE T Max(T t1, T t2, T t3, T t4, T t5);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE int 	Min(int t1, int t2);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE int 	Min(int t1, int t2, int t3);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE int 	Min(int t1, int t2, int t3, int t4);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE int 	Min(int t1, int t2, int t3, int t4, int t5);

    // Returns the largest of the given values. 
    static T3_FORCE_INLINE int 	Max(int t1, int t2);

    // Returns the largest of the given values. 
    static T3_FORCE_INLINE int 	Max(int t1, int t2, int t3);

    // Returns the largest of the given values. 
    static T3_FORCE_INLINE int 	Max(int t1, int t2, int t3, int t4);

    // Returns the largest of the given values. 
    static T3_FORCE_INLINE int 	Max(int t1, int t2, int t3, int t4, int t5);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE unsigned int Min(unsigned int t1, unsigned int t2);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE unsigned int Min(unsigned int t1, unsigned int t2, unsigned int t3);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE unsigned int Min(unsigned int t1, unsigned int t2, unsigned int t3, unsigned int t4);

    // Returns the smallest of the given values. 
    static T3_FORCE_INLINE unsigned int Min(unsigned int t1, unsigned int t2, unsigned int t3, unsigned int t4, unsigned int t5);

    // Returns the largest of the given values. 
    static T3_FORCE_INLINE unsigned int Max(unsigned int t1, unsigned int t2);

    // Returns the largest of the given values. 
    static T3_FORCE_INLINE unsigned int Max(unsigned int t1, unsigned int t2, unsigned int t3);

    // Returns the largest of the given values. 
    static T3_FORCE_INLINE unsigned int Max(unsigned int t1, unsigned int t2, unsigned int t3, unsigned int t4);

    // Returns the largest of the given values.
    static T3_FORCE_INLINE unsigned int Max(unsigned int t1, unsigned int t2, unsigned int t3, unsigned int t4, unsigned int t5);

    // Returns val clamped to the range [tMin;tMax]. 
    template<class T >
    static T3_FORCE_INLINE T clamp(T val, T tMin, T tMax);

    // Returns val clamped to the range [0;1]. 
    template<class T >
    static T3_FORCE_INLINE T saturate(T val);

    // Checks whether val is between minVal and maxVal (inclusive). 
    template<class T >
    static T3_FORCE_INLINE bool isInRange(T val, T minVal, T maxVal);

    // -----------------------Special Float Functions-----------------------
    // Returns 'f modulo d' for floats. 
    static T3_FORCE_INLINE float mod(float f, float d);

    // Checks whether f is equal to zero within a given epsilon range. 
    static T3_FORCE_INLINE bool isZero(float f, float fEpsilon = T3MATH_LARGE_EPSILON);

    // Checks whether f is equal to zero within a given epsilon range. 
    static T3_FORCE_INLINE bool isZero(double f, double fEpsilon = T3MATH_LARGE_EPSILON);

    // Checks whether f1 is equal to f2 within a given epsilon range. 
    static T3_FORCE_INLINE bool isFloatEqual(float f1, float f2, float fEpsilon = T3MATH_LARGE_EPSILON);

    // Checks whether f1 is equal to f2 within a given epsilon range. 
    static T3_FORCE_INLINE bool isFloatEqual(double f1, double f2, double fEpsilon = T3MATH_LARGE_EPSILON);

    // Returns true if f is between -Infinity and +Infinity and is not NaN. 
    static T3_FORCE_INLINE bool isFiniteNumber(float f);

    // Same as 'isFiniteNumber': Returns true if f is between -Infinity and +Infinity and is not NaN. 
    static T3_FORCE_INLINE bool isValidFloat(float f);

    // Returns true if f is NaN (Not-a-Number). In all other cases (including +/- Infinity) it will return false. 
    static T3_FORCE_INLINE bool isNaN(float f);

    // Returns a float NaN (Not-A-Number). Can be used to set values to an invalid state. 
    static T3_FORCE_INLINE float generateNaN(void);

    static T3_FORCE_INLINE float generateInfinity(void);

    // -----------------------Utility Functions-----------------------
    // Swaps the values of the variables t1 and t2.in place. 
    template<class T >
    static T3_FORCE_INLINE void swap(T &t1, T &t2);

    // Returns a value that was linearly interpolated between val1 and val2. 
    template<class T >
    static T3_FORCE_INLINE T interpolate(const T &val1, const T &val2, float fInterpolation);

    // Returns a value that was linearly interpolated between val1 and val2. 
    template<class T >
    static T3_FORCE_INLINE T interpolate(const T &val1, const T &val2, double fInterpolation);

};


#include <t3Math/core/t3Math.inl>

#endif