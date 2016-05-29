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

t3Vector3f a3Uncharted2Tonemap(t3Vector3f x)
{
    float A = 0.15f;
    float B = 0.5f;
    float C = 0.1f;
    float D = 0.2f;
    float _E = 0.02f;
    float F = 0.3f;

    return ((x * (A * x + C * B) + D * _E) / (x * (A * x + B) + D * F)) - _E / F;
}

void a3UncharedTonemap(t3Vector3f& color, float eyeAdaption)
{
    float exposureBias = 2.0f;

    // 16用于曝光控制
    t3Vector3f texColor = eyeAdaption * color / 255.0f;
    texColor = a3Uncharted2Tonemap(exposureBias * texColor);

    float W = 11.2f;
    t3Vector3f whiteScale = 1.0f / a3Uncharted2Tonemap(t3Vector3f(W));
    color = texColor * whiteScale * 255.0f;
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
        // 向y = 0平面投影, 找到一个垂直于原向量的单位向量
        float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
        v2 = t3Vector3f(-v1.z * invLen, 0.0f, v1.x * invLen);
    }
    else
    {
        // 向x = 0平面投影, 找到一个垂直于原向量的单位向量
        float invLen = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
        v2 = t3Vector3f(0.0f, v1.z * invLen, -v1.y * invLen);
    }

    // 叉积求第三个单位向量
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

float a3FovToApretureSizeDeg(float fov)
{
    return 2.0f / t3Math::tanDeg(fov / 2.0f);
}

float a3FovToApretureSizeRad(float fov)
{
    return 2.0f / t3Math::tanRad(fov / 2.0f);
}

void a3ToneMapping(t3Vector3f* colorList, int xres, int yres)
{
    // --! Thx to Denghong's implementation
#define LUM(c) (c.x*0.299 + c.y*0.587 + 0.144*c.z)
    static t3Vector3f black = t3Vector3f(0.1, 0.1, 0.1);
    float l_blk = LUM(black);

    float ll = 0.f;

    for(int i = 0; i < yres; ++i)
    {
        for(int j = 0; j < xres; ++j)
        {
            t3Vector3f cc = colorList[i * xres + j] / 255.0f;
            //use l_blk to avoid ln(0)
            ll += t3Math::log(LUM(cc) + l_blk);
        }
    }

    float l_avg_inv = 1.f / t3Math::exp(ll / (xres*yres));

    //printf("avg:%f\n", 1.f / l_avg_inv * 255);

    //0.36/0.72 will be brighter while 0.09/0.045 will be darker.
    float key = 0.18;
    float f_l = key*l_avg_inv;

    //printf("f_l:%f\n", f_l);

    for(int i = 0; i < yres; ++i)
    {
        for(int j = 0; j < xres; ++j)
        {
            t3Vector3f cc = colorList[i * xres + j] / 255.0f;

            float lo = f_l*(LUM(cc));
            //to 0~1
#define CONTROLEXP
            float ld = lo / (1.f + lo);

#ifdef CONTROLEXP
            //Lwhite控制曝光，亮度超过Lwhite的像素都会被置为纯白。
            //如果Lwhite的值非常大，则这个参数在公式中将不起任何作用，
            //如果非常小则场景将变为几乎全溢出。
            float l_w = 1.8f;
            ld *= 1 + lo / (l_w*l_w);
#endif
            cc *= (ld / (LUM(cc)));

            cc.x = t3Math::clamp(cc.x, 0.0f, 1.0f);
            cc.y = t3Math::clamp(cc.y, 0.0f, 1.0f);
            cc.z = t3Math::clamp(cc.z, 0.0f, 1.0f);

            colorList[i * xres + j] = cc * 255.0f;
        }
    }
#undef LUM
}

float a3RGB2Luminance(const a3Spectrum& rgb)
{
    const float YWeight[3] = {0.212671f, 0.715160f, 0.072169f};
    return YWeight[0] * rgb.x + YWeight[1] * rgb.y + YWeight[2] * rgb.z;
}

// from pbrt's reflection.cpp line:59-67
float a3FresnelDielectric(float cosi, float cost, const float &etai, const float &etat)
{
    float Rparl = ((etat * cosi) - (etai * cost)) /
        ((etat * cosi) + (etai * cost));
    float Rperp = ((etai * cosi) - (etat * cost)) /
        ((etai * cosi) + (etat * cost));
    return (Rparl*Rparl + Rperp*Rperp) / 2.f;
}

// from pbrt's reflection.cpp line:71-78
float a3FresnelConductor(float cosi, float cost, const float &etai, const float &etat)
{
    float Rparl = ((etat * cosi) - (etai * cost)) /
        ((etat * cosi) + (etai * cost));
    float Rperp = ((etai * cosi) - (etat * cost)) /
        ((etai * cosi) + (etat * cost));
    return (Rparl*Rparl + Rperp*Rperp) / 2.f;
}
