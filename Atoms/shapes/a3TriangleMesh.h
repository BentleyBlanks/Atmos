#ifndef A3_TRIANGLEMESH_H
#define A3_TRIANGLEMESH_H

#include <shapes/a3Shape.h>

class a3TriangleMesh :public a3Shape
{
public:
    a3TriangleMesh();

    virtual bool intersect(const a3Ray& ray, float* t) const;

    virtual t3Vector3f getNormal(const t3Vector3f& vector) const;

};

#endif