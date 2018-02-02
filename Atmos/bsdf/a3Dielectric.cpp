#include <bsdf/a3Dielectric.h>

#include <core/a3Warp.h>
#include <core/a3Record.h>
#include <core/random/a3Random.h>

a3Dielectric::a3Dielectric()
{
    // bk7
    eta = 1.5046f;

    specularReflectance = a3Spectrum::one();
    specularTransmittance = a3Spectrum::one();
}

a3Spectrum a3Dielectric::eval(const a3BSDFSamplingRecord & bRec) const
{
    float cosThetaT;
    float cosThetaI = a3CosTheta(bRec.wi);
    float cosThetaO = a3CosTheta(bRec.wo);

    float f = a3FresnelDielectric(cosThetaI, cosThetaT, eta);

    // wo in the reflect(spcular) direction, in the same side of the shading normal
    if(cosThetaI * cosThetaO >= 0)
    {
        t3Vector3f reflect = a3GetReflect(bRec.wi);

        if(t3Math::Abs(reflect.dot(bRec.wo) - 1) > A3_EPSILON_DELTA)
            return a3Spectrum::zero();

        return specularReflectance * f;
    }
    // wo in the refract(transmission) direction, in the other side of the shading normal
    else
    {
        t3Vector3f refract = a3GetRefract(bRec.wi, cosThetaT, eta);

        if(t3Math::Abs(refract.dot(bRec.wo) - 1) > A3_EPSILON_DELTA)
            return a3Spectrum::zero();

        // ray cross the surface from material inside to outside(> 0)
        float factor = cosThetaT < 0 ? 1 / eta : eta;

        // scaled to account for the solid angle compression that occurs when crossing the interface
        return specularTransmittance * (1 - f) * factor * factor;
    }
}

a3Spectrum a3Dielectric::sample(a3BSDFSamplingRecord & bRec) const
{
    float cosThetaT;
    float cosThetaI = a3CosTheta(bRec.wi);

    float f = a3FresnelDielectric(cosThetaI, cosThetaT, eta);

    // the probability of reflection and refraction depend on the fresnel value
    if(a3Random::randomFloat() <= f)
    {
        bRec.wo = a3GetReflect(bRec.wi);
        bRec.eta = 1.0f;
        bRec.pdf = f;

        return specularReflectance;
    }
    else
    {
        bRec.wo = a3GetRefract(bRec.wi, cosThetaT, eta);
        bRec.eta = cosThetaT < 0 ? eta : 1 / eta;
        bRec.pdf = 1 - f;

        // ray cross the surface from material inside to outside(> 0)
        float factor = cosThetaT < 0 ? 1 / eta : eta;

        return specularTransmittance * factor * factor;
    }
}

float a3Dielectric::pdf(const a3BSDFSamplingRecord & bRec) const
{
    float cosThetaT;
    float cosThetaI = a3CosTheta(bRec.wi);
    float cosThetaO = a3CosTheta(bRec.wo);

    float f = a3FresnelDielectric(cosThetaI, cosThetaT, eta);

    // wo in the reflect(spcular) direction, in the same side of the shading normal
    if(cosThetaI * cosThetaO >= 0)
    {
        t3Vector3f reflect = a3GetReflect(bRec.wi);

        if(t3Math::Abs(reflect.dot(bRec.wo) - 1) > A3_EPSILON_DELTA)
            return 0.0f;

        return f;
    }
    // wo in the refract(transmission) direction, in the other side of the shading normal
    else
    {
        t3Vector3f refract = a3GetRefract(bRec.wi, cosThetaT, eta);

        if(t3Math::Abs(refract.dot(bRec.wo) - 1) > A3_EPSILON_DELTA)
            return 0.0f;

        // scaled to account for the solid angle compression that occurs when crossing the interface
        return 1 - f;
    }
}

bool a3Dielectric::isDeltaDistribution() const
{
    return true;
}
