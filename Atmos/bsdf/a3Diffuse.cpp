#include <bsdf/a3Diffuse.h>
#include <t3Math/core/t3Matri4x4.h>

#include <core/a3Warp.h>
#include <core/random/a3Random.h>
#include <core/a3Record.h>
#include <core/a3Record.h>

a3Diffuse::a3Diffuse(a3Spectrum R) :R(R)
{
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

    // reflectance * pdf * cosTheta;
    return R * pdf(bRec);
}

a3Spectrum a3Diffuse::sample(a3BSDFSamplingRecord & bRec) const
{
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    if(cosThetai <= 0)
        return a3Spectrum::zero();

    static a3Random random;

    //bRec.wo = a3UniformSampleHemisphere(random.randomFloat(), random.randomFloat());
    //bRec.pdf = a3UniformHemispherePdf() * bRec.wo.y;

    // local system
    // cosine weighted
    bRec.wo = a3CosineSampleHemisphere(random.randomFloat(), random.randomFloat());
    bRec.pdf = a3CosineSampleHemispherePdf(bRec.wo);
    bRec.eta = eta;

    return R;
}

float a3Diffuse::pdf(const a3BSDFSamplingRecord & bRec) const
{
    float cosThetai = bRec.wi.dot(t3Vector3f(0, 1, 0));
    float cosThetao = bRec.wo.dot(t3Vector3f(0, 1, 0));

    if(cosThetai <= 0 || cosThetao <= 0)
        return 0.0f;

    // cosine weighted pdf
    return a3CosineSampleHemispherePdf(bRec.wo);
}

bool a3Diffuse::isDeltaDistribution() const
{
    return false;
}