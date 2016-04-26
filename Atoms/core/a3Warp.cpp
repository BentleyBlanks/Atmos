#include <core/a3Warp.h>

t3Vector2f a3SquareToUniformDisk(const float sampleU, const float sampleV)
{
    float phi, r, u, v;
    // (a,b) is now on [-1,1]ˆ2
    float a = 2 * sampleU - 1;
    float b = 2 * sampleV - 1;

    // region 1 or 2
    if(a > -b)
    {
        // region 1, also |a| > |b|
        if(a > b)
        {
            r = a;
            phi = (T3MATH_PI / 4) * (b / a);
        }
        // region 2, also |b| > |a|
        else
        {
            r = b;
            phi = (T3MATH_PI / 4) * (2 - (a / b));
        }
    }
    // region 3 or 4
    else
    {
        // region 3, also |a| >= |b|, a != 0
        if(a < b)
        {
            r = -a;
            phi = (T3MATH_PI / 4) * (4 + (b / a));
        }
        // region 4, |b| >= |a|, but a==0 and b==0 could occur.
        else
        {
            r = -b;
            if(b != 0)
                phi = (T3MATH_PI / 4) * (6 - (a / b));
            else
                phi = 0;
        }
    }

    u = r* cos(phi);
    v = r* sin(phi);
    return t3Vector2f(u, v);
}

float a3SphericalTheta(const t3Vector3f &v)
{
    return t3Math::acosRad(t3Math::clamp(v.z, -1.f, 1.f));
}

float a3SphericalPhi(const t3Vector3f &v)
{
    // 等价于atan(y/x)
    float p = t3Math::atan2Rad(v.y, v.x);
    return (p < 0.f) ? p + 2.f * T3MATH_PI : p;
}

t3Vector3f a3Tonemap(t3Vector3f x)
{
    float A = 0.15f;
    float B = 0.5f;
    float C = 0.1f;
    float D = 0.2f;
    float _E = 0.02f;
    float F = 0.3f;

    return ((x * (A * x + C * B) + D * _E) / (x * (A * x + B) + D * F)) - _E / F;
}

void a3GammaCorrection(t3Vector3f& color)
{
    t3Vector3f& toneColor = color;

    toneColor.x = t3Math::pow(toneColor.x / 255.0f, 1 / 2.2f);
    toneColor.y = t3Math::pow(toneColor.y / 255.0f, 1 / 2.2f);
    toneColor.z = t3Math::pow(toneColor.z / 255.0f, 1 / 2.2f);

    toneColor *= 255;
}

// 均匀分布半球采样
// 与sinPhi^2 + cosPhi^2球上随机均匀分布采样投影至半球, u1 u2代表球坐标两参数(可任意指定sin/cos), 返回结果即为半球随机采样方向
// 此处u1:cosTheta u2:代表占据一圆周的角度百分比
t3Vector3f a3Hemisphere(float u1, float u2)
{
    //// 1 - cosThta^2 = sinTheta
    //const float r = t3Math::sqrt(1.0f - u1 * u1);

    //// phi = 2Pi * percent
    //const float phi = 2 * T3MATH_PI * u2;

    //// x = sinTheta * cosPhi; y = sinTheta * sinPhi; z = cosTheta
    //return t3Vector3f(t3Math::cosRad(phi) * r, t3Math::sinRad(phi) * r, u1);

    const float r = t3Math::sqrt(u1);
    const float theta = 2 * T3MATH_PI * u2;

    const float x = r * t3Math::cosRad(theta);
    const float y = r * t3Math::sinRad(theta);

    return t3Vector3f(x, y, t3Math::sqrt(t3Math::Max(0.0f, 1 - u1)));
}

void a3OrthonomalSystem(const t3Vector3f& v1, t3Vector3f& v2, t3Vector3f& v3)
{
    if(t3Math::Abs(v1.x) > t3Math::Abs(v1.y))
    {
        // project to the y = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
        v2 = t3Vector3f(-v1.z * invLen, 0.0f, v1.x * invLen);
    }
    else
    {
        // project to the x = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
        v2 = t3Vector3f(0.0f, v1.z * invLen, -v1.y * invLen);
    }
    v3 = v1.getCrossed(v2);
}

bool a3SolveQuadratic(float A, float B, float C, float* t0, float* t1)
{
    // --!Copied from Mitsuba(util.cpp 447)
    /* Linear case */
    if(A == 0)
    {
        if(B != 0)
        {
            *t0 = *t1 = -C / B;
            return true;
        }
        return false;
    }

    float discrim = B * B - 4.0f * A * C;

    /* Leave if there is no solution */
    if(discrim < 0)
        return false;

    float temp, sqrtDiscrim = t3Math::sqrt(discrim);

    /* Numerically stable version of (-b (+/-) sqrtDiscrim) / (2 * a)
    *
    * Based on the observation that one solution is always
    * accurate while the other is not. Finds the solution of
    * greater magnitude which does not suffer from loss of
    * precision and then uses the identity x1 * x2 = c / a
    */
    // 通过B的状态求得更大的temp来避免出现精度丢失问题
    if(B < 0)
        temp = -0.5f * (B - sqrtDiscrim);
    else
        temp = -0.5f * (B + sqrtDiscrim);

    *t0 = temp / A;
    *t1 = C / temp;

    /* Return the results so that t0 < t1 */
    if(*t0 > *t1)
        t3Math::swap(*t0, *t1);

    return true;

}

bool a3SolveQuadraticDouble(double A, double B, double C, double* t0, double* t1)
{
    // --!Copied from Mitsuba(util.cpp 447)
    /* Linear case */
    if(A == 0)
    {
        if(B != 0)
        {
            *t0 = *t1 = -C / B;
            return true;
        }
        return false;
    }

    double discrim = B * B - 4.0f * A * C;

    /* Leave if there is no solution */
    if(discrim < 0)
        return false;

    double temp, sqrtDiscrim = t3Math::sqrtd(discrim);

    /* Numerically stable version of (-b (+/-) sqrtDiscrim) / (2 * a)
    *
    * Based on the observation that one solution is always
    * accurate while the other is not. Finds the solution of
    * greater magnitude which does not suffer from loss of
    * precision and then uses the identity x1 * x2 = c / a
    */
    // 通过B的状态求得更大的temp来避免出现精度丢失问题
    if(B < 0)
        temp = -0.5f * (B - sqrtDiscrim);
    else
        temp = -0.5f * (B + sqrtDiscrim);

    *t0 = temp / A;
    *t1 = C / temp;

    /* Return the results so that t0 < t1 */
    if(*t0 > *t1)
        t3Math::swap(*t0, *t1);

    return true;
}

float a3PowerHeuristic(int nf, float fPdf, int ng, float gPdf)
{
    float f = nf * fPdf, g = ng * gPdf;
    return (f*f) / (f*f + g*g);
}

float a3UniformSpherePdf()
{
    return 1.f / (4.f * T3MATH_PI);
}
