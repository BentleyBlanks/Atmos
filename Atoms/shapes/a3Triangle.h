#ifndef A3_TRIANGLE_H
#define A3_TRIANGLE_H

#include <shapes/a3Shape.h>

class a3Triangle:public a3Shape
{
public:
    a3Triangle();

    a3Triangle(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2);

    virtual float intersect(const a3Ray& ray) const;

    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint) const;

    t3Vector3f v0, v1, v2;

    // ¶¥µã·¨Ïß
    t3Vector3f n0, n1, n2;
};

#endif