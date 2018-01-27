#ifndef A3_WARP_H
#define A3_WARP_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>
#include <core/a3Spectrum.h>

// --!公共函数
// ----------------------------------------------Sampling----------------------------------------------
enum a3UniformSampleDiskType
{
    // --!Peter Shirley提出的变换更小的解法 https://mediatech.aalto.fi/~jaakko/T111-5310/K2013/JGT-97.pdf
    A3UNIFORM_SAMPLE_DISK_CONCENTRIC = 0,
    // 概率密度转换得到的笛卡尔表达
    A3UNIFORM_SAMPLE_DISK_CARTESIAN
};

enum a3CosineSampleHemisphereType
{
    // 于圆盘上均匀采样投影至半球满足Cosine-Weighted
    A3UNIFORM_SAMPLE_HEMISPHERE_PROJECTED = 0,
    // 概率密度转换得到的笛卡尔表达
    A3UNIFORM_SAMPLE_HEMISPHERE_CARTESIAN
};

// Uniformly sample a vector on a 2D disk
t3Vector2f a3UniformSampleDisk(float u1, float u2, a3UniformSampleDiskType type = A3UNIFORM_SAMPLE_DISK_CONCENTRIC);

// Uniformly sample a vector on the unit sphere with respect to solid angles
t3Vector3f a3UniformSampleSphere(float u1, float u2);

// Uniformly sample a vector on the unit hemisphere with respect to solid angles
t3Vector3f a3UniformSampleHemisphere(float u1, float u2);

// Sample a cosine-weighted vector on the unit hemisphere with respect to solid angles
t3Vector3f a3CosineSampleHemisphere(float u1, float u2, a3CosineSampleHemisphereType type = A3UNIFORM_SAMPLE_HEMISPHERE_PROJECTED);

// sampled cone uniformly with square sample
t3Vector3f a3UniformSampleCone(float u1, float u2, float cosThetaMax);

// Convert an uniformly distributed square sample into barycentric coordinates
t3Vector2f a3UniformSampleTriangle(float u1, float u2);

// 均匀采样球概率分布
float a3UniformSpherePdf();

// 均匀采样半球概率分布
float a3UniformHemispherePdf();

// Density of a3CosineSampleHemisphere() with respect to solid angles
float a3CosineSampleHemispherePdf(const t3Vector3f& d);

// ----------------------------------------------Post Effect----------------------------------------------
// 局部空间色调映射
void a3ToneMapping(t3Vector3f* colorList, int startX, int startY, int localWidth, int localHeight, int width, int height);

// 色调映射(需要保证colorList指针已指向线性空间 大小为width * height)
void a3ToneMapping(t3Vector3f* colorList, int width, int height);

// float数组的三通道Buffer
void a3ToneMapping(float* buffer, int width, int height);

// 未启用的Tone Mapping
void a3UncharedTonemap(t3Vector3f& color, float eyeAdaption = 4);

// 伽马矫正(三分量)
void a3GammaCorrection(float& r, float&g, float& b);

// 伽马矫正
void a3GammaCorrection(t3Vector3f& color);



// ----------------------------------------------Math----------------------------------------------
// 球坐标Theta:[0, 2pi]
float a3SphericalTheta(const t3Vector3f &v);

// 球坐标Phi:[-pi, +pi]
float a3SphericalPhi(const t3Vector3f &v);

// 给定v1坐标轴 与v2v3构建一正交坐标系(返回单位向量)
void a3OrthonomalSystem(const t3Vector3f& v1, t3Vector3f& v2, t3Vector3f& v3);

// 计算判别式求解二次方程f(若有解则t0 <= t1)
bool a3SolveQuadratic(float A, float B, float C, float* t0, float* t1);

// 计算判别式求解二次方程d(若有解则t0 <= t1)
bool a3SolveQuadraticDouble(double A, double B, double C, double* t0, double* t1);

// 多重重要性采样
float a3MiWeight(float fPdf, float gPdf);

// [Tools]转Fov为apretureWidth/Height(度)
float a3FovToApretureSizeDeg(float fov);

// [Tools]转Fov为apretureWidth/Height(弧度)
float a3FovToApretureSizeRad(float fov);

// RGBSpectrum转为亮度/明度
float a3RGB2Luminance(const a3Spectrum& rgb);

// 导体的菲涅尔反射项
float a3FresnelDielectric(float cosi, float cost, const float &etai, const float &etat);

// 绝缘体的菲涅尔反射项
float a3FresnelConductor(float cosi, float cost, const float &etai, const float &etat);
#endif