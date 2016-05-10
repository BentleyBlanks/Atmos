﻿#ifndef A3_INTERSECTION_H
#define A3_INTERSECTION_H

#include <core/a3Settings.h>
#include <t3Math/core/t3Vector3.h>

class a3Shape;

class a3Intersection
{
public:
    a3Intersection(): shape(NULL), t(FLT_MAX), u(0), v(0){}

    bool isValid() const;

    t3Vector3f getNormal() const;

    bool isEmitter() const;

    // Distance traveled along the ray
    float t;

    // Intersection point's uv coordinate
    float u, v;

    // Pointer to the associated shape
    a3Shape *shape;

    // Intersection point in 3D coordinates
    t3Vector3f p;
};

#endif 