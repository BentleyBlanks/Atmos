#include <bsdf/a3RoughConductor.h>

#include <core/a3Settings.h>
#include <core/a3Warp.h>
#include <core/a3Record.h>
#include <core/random/a3Random.h>

a3RoughConductor::a3RoughConductor()
{
    R = a3Spectrum::one();

    eta = a3Spectrum::zero();
    k = a3Spectrum::one();

    type = a3Microfacet::a3MicrofacetType::Beckmann;
    alphaX = 0.1f;
    alphaY = 0.1f;
}

a3Spectrum a3RoughConductor::eval(const a3BSDFSamplingRecord & bRec) const
{		
    /* Stop if this component was not requested */
    if(a3CosTheta(bRec.wi) <= 0 ||
       a3CosTheta(bRec.wo) <= 0)
        return a3Spectrum(0.0f);

    /* Calculate the reflection half-vector */
    t3Vector3f H = (bRec.wo + bRec.wi).getNormalized();

    /* Construct the microfacet distribution matching the
    roughness values at the current surface position. */
    a3Microfacet distr(alphaX, alphaY, type);

    /* Evaluate the microfacet normal distribution */
    const float D = distr.D(H);
    if(D == 0)
        return a3Spectrum(0.0f);

    /* Fresnel factor */
    const a3Spectrum F = a3FresnelConductor(bRec.wi.dot(H), eta, k) * R;

    /* Smith's shadow-masking function */
    const float G = distr.G(bRec.wi, bRec.wo, H);

    /* Calculate the total amount of reflection */
    return F * D * G / (4.0f * a3CosTheta(bRec.wi));
}

a3Spectrum a3RoughConductor::sample(a3BSDFSamplingRecord & bRec) const
{
    if(a3CosTheta(bRec.wi) < 0)
        return a3Spectrum(0.0f);

    /* Construct the microfacet distribution matching the
    roughness values at the current surface position. */
    a3Microfacet distr(alphaX, alphaY, type);

    /* Sample M, the microfacet normal */
    float pdf;
    t3Vector3f m = distr.sample(bRec.wi, pdf);

    if(pdf == 0)
        return a3Spectrum(0.0f);

    /* Perfect specular reflection based on the microfacet normal */
    bRec.wo = a3GetReflect(bRec.wi, m);
    bRec.eta = 1.0f;

    /* Side check */
    if(a3CosTheta(bRec.wo) <= 0)
        return a3Spectrum(0.0f);

    a3Spectrum F = a3FresnelConductor(bRec.wi.dot(m), eta, k) * R;

    float weight = distr.D(m) * distr.G(bRec.wi, bRec.wo, m)
            * bRec.wi.dot(m) / (pdf * a3CosTheta(bRec.wi));

    /* Jacobian of the half-direction mapping */
    bRec.pdf = pdf / (4.0f * bRec.wo.dot(m));

    return F * weight;
}

float a3RoughConductor::pdf(const a3BSDFSamplingRecord & bRec) const
{
    if(a3CosTheta(bRec.wi) <= 0 ||
       a3CosTheta(bRec.wo) <= 0)
        return 0.0f;

    /* Calculate the reflection half-vector */
    t3Vector3f H = (bRec.wo + bRec.wi).getNormalized();

    /* Construct the microfacet distribution matching the
    roughness values at the current surface position. */
    a3Microfacet distr(alphaX, alphaY, type);

    return distr.pdf(bRec.wi, H) / (4 * t3Math::Abs(bRec.wo.dot(H)));
}

bool a3RoughConductor::isDeltaDistribution() const
{
    return false;
}
