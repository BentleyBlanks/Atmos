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

a3Spectrum a3Diffuse::sample(a3BSDFSamplingRecord & bRec) const
{
    static a3Random random;

    bRec.wo = a3UniformSampleHemisphere(random.randomFloat(), random.randomFloat());
    bRec.pdf = T3MATH_INV_PI;
    bRec.eta = eta;

    return R;
}

float a3Diffuse::pdf(const a3BSDFSamplingRecord & bRec) const
{
    return T3MATH_INV_PI;
}

bool a3Diffuse::isDeltaDistribution() const
{
    return false;
}