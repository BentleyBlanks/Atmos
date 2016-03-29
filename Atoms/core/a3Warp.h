#ifndef A3_WARP_H
#define A3_WARP_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>

// --!公共函数

// 笛卡尔坐标系转换为极坐标系
t3Vector2f a3SquareToUniformDisk(const float sampleU, const float sampleV);

// 色调映射
t3Vector3f a3Tonemap(t3Vector3f color);

// 伽马矫正
void a3GammaCorrection(t3Vector3f& color);

// 球坐标Theta:[0, 2pi]
float a3SphericalTheta(const t3Vector3f &v);

// 球坐标Phi:[-pi, +pi]
float a3SphericalPhi(const t3Vector3f &v);

// 均匀分布半球采样
t3Vector3f a3Hemisphere(float u1, float u2);

// 给定v1坐标轴 与v2v3构建一正交坐标系
void a3OrthonomalSystem(const t3Vector3f& v1, t3Vector3f& v2, t3Vector3f& v3);

// 计算判别式求解二次方程(若有解则t0 <= t1)
bool a3SolveQuadratic(float A, float B, float C, float* t0, float* t1);

bool a3SolveQuadraticDouble(double A, double B, double C, double* t0, double* t1);

#endif