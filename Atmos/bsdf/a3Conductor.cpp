#include <bsdf/a3Conductor.h>

#include <core/a3Record.h>
#include <core/a3Warp.h>

a3Conductor::a3Conductor()
{
    R = a3Spectrum::one();

    eta = a3Spectrum::zero();
    k = a3Spectrum::one();
}

a3Spectrum a3Conductor::eval(const a3BSDFSamplingRecord & bRec) const
{
    // Verify that the provided direction pair matches an ideal specular reflection
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    float cosThetao = bRec.wo.dot(t3Vector3f(0, 1, 0));

    t3Vector3f reflect = a3GetReflect(bRec.wi);

    if(cosThetai <= 0 || 
       cosThetao <= 0 ||
       t3Math::Abs(reflect.dot(bRec.wo) -1 > A3_EPSILON_DELTA))
        return a3Spectrum::zero();


    return R * a3FresnelConductor(cosThetai, eta, k);
}

a3Spectrum a3Conductor::sample(a3BSDFSamplingRecord & bRec) const
{
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    if(cosThetai <= 0)
        return a3Spectrum::zero();

    bRec.wo = a3GetReflect(bRec.wi);
    bRec.eta = 1.0f;
    bRec.pdf = 1.0f;

    return R * a3FresnelConductor(cosThetai, eta, k);
}

float a3Conductor::pdf(const a3BSDFSamplingRecord & bRec) const
{    
    // Verify that the provided direction pair matches an ideal specular reflection
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    float cosThetao = bRec.wo.dot(t3Vector3f(0, 1, 0));

    t3Vector3f reflect = a3GetReflect(bRec.wi);

    if(cosThetai <= 0 ||
       cosThetao <= 0 ||
       t3Math::Abs(reflect.dot(bRec.wo) - 1 > A3_EPSILON_DELTA))
        return 0.0f;

    return 1.0f;
}

bool a3Conductor::isDeltaDistribution() const
{
    return true;
}
