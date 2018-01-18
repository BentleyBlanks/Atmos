#ifndef A3_SHAPE_H
#define A3_SHAPE_H

#include <t3Math/core/t3Vector3.h>
#include <t3Math/core/t3Matri4x4.h>
#include <core/a3Settings.h>
#include <core/a3Spectrum.h>
#include <core/a3AABB.h>
#include <bsdf/a3BSDF.h>

class a3Ray;
class a3ShapeSamplingRecord;
class a3Shape
{
public:
    a3Shape(const t3Matrix4x4& objectToWorld, const t3Matrix4x4& worldToObject);

    virtual ~a3Shape();

    // ray intersection test for visibility queries
    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v) const = 0;

    // compute the area
    virtual float area() const = 0;

    // Sample a point on the surface of this shape instance
    virtual void sample(a3ShapeSamplingRecord& sRec) const = 0;

    // Query the probability density of sample() for a particular point on the surface.
    virtual float pdf(const a3ShapeSamplingRecord& sRec) const;

    // return the normal vec from the point be hitted
    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const = 0;

    // bsdf set/get
    virtual void setBSDF(a3BSDF* bsdf);

    virtual a3BSDF* getBSDF() const;

    t3Matrix4x4 objectToWorld, worldToObject;

    // Axis-Aligned Bounding Box
    a3AABB aabb;

    std::string name;

    a3BSDF* bsdf;
};

#endif
