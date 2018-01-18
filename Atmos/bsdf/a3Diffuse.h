#ifndef A3_DIFFUSE_BSDF_H
#define A3_DIFFUSE_BSDF_H

#include <bsdf/a3BSDF.h>

class a3Diffuse : public a3BSDF
{
public:
    a3Diffuse(a3Spectrum R = a3Spectrum::one());

    // Sample the BSDF and return the importance weight
    virtual a3Spectrum sample(a3BSDFSamplingRecord& bRec) const;

    //  Compute the probability of sampling wo (given wi)
    virtual float pdf(const a3BSDFSamplingRecord& bRec) const;

    // if delta distribution
    virtual bool isDeltaDistribution() const;

    // Reflectance of Lambertion
    a3Spectrum R;
};

#endif