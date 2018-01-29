#include <core/a3Warp.h>

// ----------------------------------------------Sampling----------------------------------------------
t3Vector2f a3UniformSampleDisk(float u1, float u2, a3UniformSampleDiskType type)
{
    if(type == A3UNIFORM_SAMPLE_DISK_CONCENTRIC)
    {
/*        float phi, r, u, v;
        // (a,b) is now on [-1,1]ˆ2
        float a = 2 * u1 - 1;
        float b = 2 * u2 - 1;

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

        u = r * t3Math::cosRad(phi);
        v = r * t3Math::sinRad(phi);
        return t3Vector2f(u, v);
*/
        float r1 = 2.0f * u1 - 1.0f;
        float r2 = 2.0f * u2 - 1.0f;

        /* Modified concencric map code with less branching (by Dave Cline), see
        http://psgraphics.blogspot.ch/2011/01/improved-code-for-concentric-map.html */
        float phi, r;
        if(r1 == 0 && r2 == 0)
        {
            r = phi = 0;
        }
        else if(r1*r1 > r2*r2)
        {
            r = r1;
            phi = (T3MATH_PI / 4.0f) * (r2 / r1);
        }
        else
        {
            r = r2;
            phi = (T3MATH_PI / 2.0f) - (r1 / r2) * (T3MATH_PI / 4.0f);
        }

        float cosPhi = t3Math::cosRad(phi), sinPhi = t3Math::sinRad(phi);

        return t3Vector2f(r * cosPhi, r * sinPhi);
    }
    else if(type == A3UNIFORM_SAMPLE_DISK_CARTESIAN)
    {
        float r = t3Math::sqrt(u1);

        float theta = 2.0f * T3MATH_PI * u2;

        float u = r * t3Math::cosRad(theta);
        float v = r * t3Math::sinRad(theta);

        return t3Vector2f(u, v);
    }
    else
    {
        // a3Log
        return t3Vector2f();
    }
}

t3Vector3f a3UniformSampleSphere(float u1, float u2)
{    
    // z=1-(1-2\xi^2)
    float z = 1.f - 2.f * u1;
    float r = t3Math::sqrt(t3Math::Max(0.f, 1.f - z*z));

    float phi = 2.f * T3MATH_PI * u2;
    float x = r * t3Math::cosRad(phi);
    float y = r * t3Math::sinRad(phi);

    // to left-handed
    return t3Vector3f(x, z, y);
}

// 均匀分布半球采样
t3Vector3f a3UniformSampleHemisphere(float u1, float u2)
{
    float z = u1;
    float r = t3Math::sqrt(t3Math::Max(0.f, 1.f - z*z));
    float phi = 2 * T3MATH_PI * u2;
    float x = r * t3Math::cosRad(phi);
    float y = r * t3Math::sinRad(phi);

    // to left-handed
    return t3Vector3f(x, z, y);
}

// Cosine-Weighted半球采样
t3Vector3f a3CosineSampleHemisphere(float u1, float u2, a3CosineSampleHemisphereType type)
{
    if(type == A3UNIFORM_SAMPLE_HEMISPHERE_PROJECTED)
    {
        t3Vector2f p = a3UniformSampleDisk(u1, u2);

        float z = t3Math::sqrt(t3Math::Max(1e-6f, 1.f - p.x*p.x - p.y*p.y));

        // to left-handed
        return t3Vector3f(p.x, z, p.y);
    }
    else if(type == A3UNIFORM_SAMPLE_HEMISPHERE_CARTESIAN)
    {
        float sintheta = t3Math::sinRad(0.5f * t3Math::acosRad(1 - 2 * u1));
        float costheta = t3Math::cosRad(0.5f * t3Math::acosRad(1 - 2 * u1));

        float v = 2 * T3MATH_PI * u2;

        float x = sintheta * t3Math::cosRad(v);
        float y = sintheta * t3Math::sinRad(v);

        float z = costheta;

        // to left-handed
        return t3Vector3f(x, z, y);
    }
    else
    {
        // a3Log
        return t3Vector3f();
    }
}

t3Vector3f a3UniformSampleCone(float u1, float u2, float cosThetaMax)
{    
    // cos\theta = 1 - \xi + \xi * costhetamax
    float costheta = 1.0f - u1 + u1 * cosThetaMax;
    float sintheta = t3Math::sqrt(1.0f - costheta*costheta);
    // \phi = 2 \pi psi
    float phi = 2 * T3MATH_PI * u2;

    float x = t3Math::cosRad(phi) * sintheta;
    float y = t3Math::sinRad(phi) * sintheta;
    float z = costheta;

    // to left-handed
    return t3Vector3f(x, z, y);
}

t3Vector2f a3UniformSampleTriangle(float u1, float u2)
{
    float sqrtu1 = t3Math::sqrt(u1);

    float u = 1.f - sqrtu1;
    float v = u2 * sqrtu1;

    return t3Vector2f(u, v);
}

float a3UniformSpherePdf()
{
    return 1.f / (4.f * T3MATH_PI);
}

float a3UniformHemispherePdf()
{
    return 1 / (2.f * T3MATH_PI);
}

float a3CosineSampleHemispherePdf(const t3Vector3f & d)
{
    // in local system return 1/pi * cosTheta
    return T3MATH_INV_PI * d.dot(t3Vector3f(0, 1, 0));
}

// ----------------------------------------------Post Effect----------------------------------------------
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

    toneColor.x = t3Math::pow(toneColor.x, 1 / 2.2f);
    toneColor.y = t3Math::pow(toneColor.y, 1 / 2.2f);
    toneColor.z = t3Math::pow(toneColor.z, 1 / 2.2f);

    //toneColor *= 255;
}

float a3RGBToSRGBComponent(float value)
{
    if(value <= (float) 0.0031308)
        return (float) 12.92 * value;
    return (float) 1.055  * std::pow(value, (float) (1.0 / 2.4)) - (float) 0.055;
}

void a3GammaCorrection(float& r, float&g, float& b)
{
    //r = t3Math::pow(r, 1 / 2.2f);
    //g = t3Math::pow(g, 1 / 2.2f);
    //b = t3Math::pow(b, 1 / 2.2f);

    r = a3RGBToSRGBComponent(r);
    g = a3RGBToSRGBComponent(g);
    b = a3RGBToSRGBComponent(b);
}

void a3OrthonomalSystem(const t3Vector3f& v1, t3Vector3f& v2, t3Vector3f& v3)
{
    if(t3Math::Abs(v1.x) > t3Math::Abs(v1.y))
    {
        // 向y = 0平面投影, 找到一个垂直于原向量的单位向量
        float invLen = 1.f / t3Math::sqrt(v1.x * v1.x + v1.z * v1.z);
        v2 = t3Vector3f(-v1.z * invLen, 0.0f, v1.x * invLen);
    }
    else
    {
        // 向x = 0平面投影, 找到一个垂直于原向量的单位向量
        float invLen = 1.0f / t3Math::sqrt(v1.y * v1.y + v1.z * v1.z);
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

float a3MiWeight(float pdfA, float pdfB)
{
    pdfA *= pdfA; pdfB *= pdfB;
    return pdfA / (pdfA + pdfB);
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
    a3ToneMapping(colorList, 0, 0, xres, yres, xres, yres);
}

void a3ToneMapping(float* buffer, int width, int height)
{
    // --! Thx to Denghong's implementation
#define LUM(c) (c.x*0.299f + c.y*0.587f + 0.144f*c.z)
    static t3Vector3f black = t3Vector3f(0.1f, 0.1f, 0.1f);
    float l_blk = LUM(black);

    float ll = 0.f;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            t3Vector3f cc(buffer[(i * width + j) * 3 + 0],
                          buffer[(i * width + j) * 3 + 1],
                          buffer[(i * width + j) * 3 + 2]);

            //use l_blk to avoid ln(0)
            ll += t3Math::log(LUM(cc) + l_blk);
        }
    }

    float l_avg_inv = 1.f / t3Math::exp(ll / (width * height));

    //0.36/0.72 will be brighter while 0.09/0.045 will be darker.
    float key = 0.18f;
    float f_l = key*l_avg_inv;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            t3Vector3f cc(buffer[(i * width + j) * 3 + 0],
                          buffer[(i * width + j) * 3 + 1],
                          buffer[(i * width + j) * 3 + 2]);

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

            buffer[(i * width + j) * 3 + 0] = cc.x;
            buffer[(i * width + j) * 3 + 1] = cc.y;
            buffer[(i * width + j) * 3 + 2] = cc.z;
        }
    }
#undef LUM
}

void a3ToneMapping(t3Vector3f* colorList, int startX, int startY, int localWidth, int localHeight, int width, int height)
{
    // --! Thx to Denghong's implementation
#define LUM(c) (c.x*0.299f + c.y*0.587f + 0.144f*c.z)
    static t3Vector3f black = t3Vector3f(0.1f, 0.1f, 0.1f);
    float l_blk = LUM(black);

    float ll = 0.f;

    for(int i = startY; i < startY + localHeight; ++i)
    {
        for(int j = startX; j < startX + localWidth; ++j)
        {
            t3Vector3f cc = colorList[i * width + j];
            //use l_blk to avoid ln(0)
            ll += t3Math::log(LUM(cc) + l_blk);
        }
    }

    float l_avg_inv = 1.f / t3Math::exp(ll / (localWidth*localHeight));

    //printf("avg:%f\n", 1.f / l_avg_inv * 255);

    //0.36/0.72 will be brighter while 0.09/0.045 will be darker.
    float key = 0.18f;
    float f_l = key*l_avg_inv;

    //printf("f_l:%f\n", f_l);

    for(int i = 0; i < startY + localHeight; ++i)
    {
        for(int j = 0; j < startX + localWidth; ++j)
        {
            t3Vector3f cc = colorList[i * width + j];

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

            colorList[i * width + j] = cc;
        }
    }
#undef LUM
}

float a3RGB2Luminance(const a3Spectrum& rgb)
{
    const float YWeight[3] = {0.212671f, 0.715160f, 0.072169f};
    return YWeight[0] * rgb.x + YWeight[1] * rgb.y + YWeight[2] * rgb.z;
}

//--!Deprecated fresnel complementation from pbrt2
//// from pbrt's reflection.cpp line:59-67
//float a3FresnelDielectric(float cosi, float cost, const float &etai, const float &etat)
//{
//    float Rparl = ((etat * cosi) - (etai * cost)) /
//        ((etat * cosi) + (etai * cost));
//    float Rperp = ((etai * cosi) - (etat * cost)) /
//        ((etai * cosi) + (etat * cost));
//    return (Rparl*Rparl + Rperp*Rperp) / 2.f;
//}
//
//// from pbrt's reflection.cpp line:71-78
//float a3FresnelConductor(float cosi, float cost, const float &etai, const float &etat)
//{
//    float Rparl = ((etat * cosi) - (etai * cost)) /
//        ((etat * cosi) + (etai * cost));
//    float Rperp = ((etai * cosi) - (etat * cost)) /
//        ((etai * cosi) + (etat * cost));
//    return (Rparl*Rparl + Rperp*Rperp) / 2.f;
//}

//--!would be replaced by a full complementation of a3Spectrum
a3Spectrum a3SqrtSpectrum(const a3Spectrum & s)
{
    a3Spectrum t(s);

    t.x = t3Math::sqrt(t.x);
    t.y = t3Math::sqrt(t.y);
    t.z = t3Math::sqrt(t.z);

    return t;
}

a3Spectrum a3FresnelConductor(float cosThetaI, const a3Spectrum & eta, const a3Spectrum & k)
{	
    /* Modified from "Optics" by K.D. Moeller, University Science Books, 1988 */

    float cosThetaI2 = cosThetaI*cosThetaI,
        sinThetaI2 = 1 - cosThetaI2,
        sinThetaI4 = sinThetaI2*sinThetaI2;

    a3Spectrum temp1 = eta*eta - k*k - a3Spectrum(sinThetaI2),
        a2pb2 = a3SqrtSpectrum(temp1*temp1 + k*k*eta*eta * 4),
        a = a3SqrtSpectrum((a2pb2 + temp1) * 0.5f);

    a3Spectrum term1 = a2pb2 + a3Spectrum(cosThetaI2),
        term2 = a*(2 * cosThetaI);

    a3Spectrum Rs2 = (term1 - term2) / (term1 + term2);

    a3Spectrum term3 = a2pb2*cosThetaI2 + a3Spectrum(sinThetaI4),
        term4 = term2*sinThetaI2;

    a3Spectrum Rp2 = Rs2 * (term3 - term4) / (term3 + term4);

    return 0.5f * (Rp2 + Rs2);
}

float a3FresnelDielectric(float _cosThetaI, float & _cosThetaT, float eta)
{
    if(eta == 1)
    {
        _cosThetaT = -_cosThetaI;
        return 0.0f;
    }

    // Using Snell's law, calculate the squared sine of the
    // angle between the normal and the transmitted ray
    float scale = (_cosThetaI > 0) ? 1 / eta : eta,
        cosThetaTSqr = 1 - (1 - _cosThetaI*_cosThetaI) * (scale*scale);

    // Handle for total internal reflection
    if(cosThetaTSqr <= 0.0f)
    {
        _cosThetaT = 0.0f;
        return 1.0f;
    }

    // Find the absolute cosines of the incident/transmitted rays
    float cosThetaI = t3Math::Abs(_cosThetaI);
    float cosThetaT = t3Math::sqrt(cosThetaTSqr);

    float Rs = (cosThetaI - eta * cosThetaT) / 
               (cosThetaI + eta * cosThetaT);
    float Rp = (eta * cosThetaI - cosThetaT) / 
               (eta * cosThetaI + cosThetaT);

    // maybe from interior to exterior
    _cosThetaT = (_cosThetaI > 0) ? -cosThetaT : cosThetaT;

    // No polarization -- return the unpolarized reflectance
    return 0.5f * (Rs * Rs + Rp * Rp);
}

t3Vector3f a3GetReflect(const t3Vector3f& wi)
{
    // -wo + 2 * dot(wo, normal) * normal
    // left-handed->normal(0, 1, 0)
    return t3Vector3f(-wi.x, wi.y, -wi.z);
}

t3Vector3f a3GetRefract(const t3Vector3f & wi, float cosThetaT, float eta)
{
    float scale = -(cosThetaT < 0 ? 1 / eta : eta);
    return t3Vector3f(scale*wi.x, cosThetaT, scale*wi.z);
}
