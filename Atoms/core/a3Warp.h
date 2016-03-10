#ifndef A3_WARP_H
#define A3_WARP_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>

// ¹«¹²º¯Êý
t3Vector2f a3SquareToUniformDisk(const float sampleU, const float sampleV);

t3Vector3f a3Tonemap(t3Vector3f x);

// [0, 2pi]
float a3SphericalTheta(const t3Vector3f &v);

// [-pi, +pi]
float a3SphericalPhi(const t3Vector3f &v);

#endif