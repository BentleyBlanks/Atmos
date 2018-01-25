#ifndef A3_SPHERE_H
#define A3_SPHERE_H

#include <shapes/a3Shape.h>

class a3Sphere : public a3Shape
{
public:
    a3Sphere(const t3Matrix4x4& shapeToWorld, const float radius);

    virtual ~a3Sphere();

    // ray intersection test for visibility queries
    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v) const;

    // compute the area
    virtual float area() const;

    // Sample a point on the surface of this shape instance
    virtual void sample(a3ShapeSamplingRecord& sRec) const;

    // return the normal vec from the point be hitted
    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;

    float radius;

    // initted by objectToWorld
    t3Vector3f center;
};

#endif