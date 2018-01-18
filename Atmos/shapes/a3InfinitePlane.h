#ifndef A3_INFINITE_PLANE_H
#define A3_INFINITE_PLANE_H

#include <shapes/a3Shape.h>

class a3ShapeSamplingRecord;

// For Testing, not as the shape of light 
class a3InfinitePlane :public a3Shape
{
public:
    a3InfinitePlane(const t3Matrix4x4& objectToWorld, const t3Matrix4x4& worldToObject);

    virtual ~a3InfinitePlane();

    // ray intersection test for visibility queries
    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v) const;

    // compute the area
    virtual float area() const;

    // Sample a point on the surface of this shape instance
    virtual void sample(a3ShapeSamplingRecord& sRec) const;

    // Query the probability density of sample() for a particular point on the surface.
    virtual float pdf(const a3ShapeSamplingRecord& sRec) const;

    // return the normal vec from the point be hitted
    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;
};

#endif 