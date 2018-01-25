#ifndef A3_DISK_H
#define A3_DISK_H

#include <shapes/a3Shape.h>

class a3ShapeSamplingRecord;

class a3Disk :public a3Shape
{
public:
    a3Disk(const t3Matrix4x4& shapeToWorld, const float radius);

    virtual ~a3Disk();

    // ray intersection test for visibility queries
    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v) const;

    // compute the area
    virtual float area() const;

    // Sample a point on the surface of this shape instance
    virtual void sample(a3ShapeSamplingRecord& sRec) const;

    // return the normal vec from the point be hitted
    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;

    float radius;
};

#endif