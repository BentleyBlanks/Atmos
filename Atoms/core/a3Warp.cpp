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