#ifndef A3_INTERSECTION_H
#define A3_INTERSECTION_H

#include <core/a3Settings.h>
#include <core/a3Spectrum.h>
#include <t3Math/core/t3Vector2.h>

class a3Shape;
class a3BSDF;

// -------------------------BSDF-------------------------
// Container for all information related to a surface intersection
class a3IntersectRecord
{
public:
    a3IntersectRecord(): shape(NULL), t(FLT_MAX), u(0), v(0), vtu(0), vtv(0), vtw(0) {}

    // returns radiance emitted into direction d
    a3Spectrum Le(const t3Vector3f& d) const;

    // ray's t is valid or not
    bool isValid() const;

    // Is the intersected shape also a emitter?
    bool isLight() const;

    // get the normal vector on hit point 
    t3Vector3f getNormal() const;

    // get the hit shape's bsdf
    const a3BSDF* getBSDF() const;

    /// Distance traveled along the ray
    float t;

    /// Intersection point's uv coordinate
    float u, v;

    /// Intersection point's uvw texture coordinate
    float vtu, vtv, vtw;

    /// Pointer to the associated shape
    a3Shape *shape;

    /// Intersection point in 3D coordinates
    t3Vector3f p;
};

// Generic sampling record for shapes
class a3ShapeSamplingRecord
{
public:
    a3ShapeSamplingRecord() : pdf(0.0f){}

    /// Sampled position
    t3Vector3f p;

    /// Sampled surface normal
    t3Vector3f normal;

    /// Probability density at the sample
    float pdf;
};

// All information that is required to sample or query a BSDF
class a3BSDFSamplingRecord
{
public:
    // parameter not required in constructor would be updated when BSDF Sampling
    a3BSDFSamplingRecord(const a3IntersectRecord &its, const t3Vector3f& wi, const t3Vector3f& wo)
        :its(its), wi(wi), wo(wo) {}

    // Reference to the underlying surface interaction
    const a3IntersectRecord &its;

    // Normalized incident direction in local coordinates
    t3Vector3f wi;

    // Normalized outgoing direction in local coordinates
    t3Vector3f wo;

    // Relative index of refraction in the sampled direction
    float eta;

    /// Probability density at the sample
    float pdf;
};

// -------------------------Light-------------------------
// Record for solid-angle based area sampling techniques
class a3LightSamplingRecord
{
public:
    // parameter not required in constructor would be updated when Light Sampling
    a3LightSamplingRecord(const t3Vector3f& hitPoint, const t3Vector3f& hitNormal)
        :hitPoint(hitPoint), hitNormal(hitNormal), distance(0.0f), pdf(0.0f) {}

    // Reference point for direct sampling
    t3Vector3f hitPoint;

    // normal vector associated with the reference point
    t3Vector3f hitNormal;

    /// Sampled position
    t3Vector3f p;

    /// Distance from the reference point to the target direction
    float distance;

    /// Unit direction from the reference point to the target direction
    t3Vector3f d;

    /// Probability density at the sample
    float pdf;

    /// Sampled surface normal
    t3Vector3f normal;

    // 2D sample position associated with the record
    //t3Vector2f uv;
};

#endif