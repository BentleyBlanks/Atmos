#ifndef A3_WARP_H
#define A3_WARP_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>

t3Vector2f squareToUniformDisk(const float sampleU, const float sampleV);

t3Vector3f tonemap(t3Vector3f x);

#endif