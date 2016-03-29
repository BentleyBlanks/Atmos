#ifndef A3_WARP_H
#define A3_WARP_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>

// --!��������

// �ѿ�������ϵת��Ϊ������ϵ
t3Vector2f a3SquareToUniformDisk(const float sampleU, const float sampleV);

// ɫ��ӳ��
t3Vector3f a3Tonemap(t3Vector3f color);

// ٤�����
void a3GammaCorrection(t3Vector3f& color);

// ������Theta:[0, 2pi]
float a3SphericalTheta(const t3Vector3f &v);

// ������Phi:[-pi, +pi]
float a3SphericalPhi(const t3Vector3f &v);

// ���ȷֲ��������
t3Vector3f a3Hemisphere(float u1, float u2);

// ����v1������ ��v2v3����һ��������ϵ
void a3OrthonomalSystem(const t3Vector3f& v1, t3Vector3f& v2, t3Vector3f& v3);

// �����б�ʽ�����η���(���н���t0 <= t1)
bool a3SolveQuadratic(float A, float B, float C, float* t0, float* t1);

bool a3SolveQuadraticDouble(double A, double B, double C, double* t0, double* t1);

#endif