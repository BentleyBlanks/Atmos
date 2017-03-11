#ifndef A3_CONDUCTOR_BSDF_H
#define A3_CONDUCTOR_BSDF_H

#include <bsdf/a3BSDF.h>

class a3Conductor : public a3BSDF
{
public:
    a3Conductor(a3Spectrum R = a3Spectrum::one());

    virtual a3Spectrum sample(const t3Vector3f& wi, t3Vector3f& wo, float* pdf, const a3Intersection& its) const;

    virtual a3Spectrum eval(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const;

    virtual bool isDiracDistribution() const;

    virtual float pdf(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const;

    // --!脏……
    //bool isReflection() const;

    a3Spectrum R;
};

#endif