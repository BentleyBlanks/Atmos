#include <bsdf/a3Diffuse.h>
#include <t3Math/core/t3Matri4x4.h>

#include <core/a3Warp.h>
#include <core/random/a3Random.h>
#include <core/a3Record.h>
#include <core/a3Record.h>

a3Diffuse::a3Diffuse()
{
    R = a3Spectrum::one();

    name = "a3Diffuse";
}

a3Spectrum a3Diffuse::eval(const a3BSDFSamplingRecord & bRec) const
{
    // shading coordinates
    // guarantee wi, normal, wo in the same side
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    float cosThetao = bRec.wo.dot(t3Vector3f(0, 1, 0));

    if(cosThetai <= 0 || cosThetao <= 0)
        return a3Spectrum::zero();

    // reflectance * bsdf(wi, wo);
    return R * pdf(bRec);
}

a3Spectrum a3Diffuse::sample(a3BSDFSamplingRecord & bRec) const
{
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    if(cosThetai <= 0)
        return a3Spectrum::zero();

    //bRec.wo = a3UniformSampleHemisphere(a3Random::randomFloat(), a3Random::randomFloat());
    //bRec.pdf = a3UniformHemispherePdf();

    // local system
    // cosine weighted
    bRec.wo = a3CosineSampleHemisphere(a3Random::randomFloat(), a3Random::randomFloat()).getNormalized();
    bRec.pdf = a3CosineSampleHemispherePdf(bRec.wo);
    bRec.eta = 1.0f;

    //return R * 2 * bRec.wo.y;
    return R;
}

float a3Diffuse::pdf(const a3BSDFSamplingRecord & bRec) const
{
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    float cosThetao = bRec.wo.dot(t3Vector3f(0, 1, 0));

    if(cosThetai <= 0 || cosThetao <= 0)
        return 0.0f;

    // cosine weighted pdf
    //return a3UniformHemispherePdf();
    return a3CosineSampleHemispherePdf(bRec.wo);
}

bool a3Diffuse::isDeltaDistribution() const
{
    return false;
}