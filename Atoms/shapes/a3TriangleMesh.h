﻿#ifndef A3_TRIANGLEMESH_H
#define A3_TRIANGLEMESH_H

#include <shapes/a3Shape.h>

class a3TriangleMesh :public a3Shape
{
public:
    a3TriangleMesh();

    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v, float* vtu, float* vtv) const;

    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;
};

#endif 