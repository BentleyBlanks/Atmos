#include <bsdf/a3Microfacet.h>

#include <core/a3Settings.h>
#include <core/a3Warp.h>
#include <core/random/a3Random.h>

a3Microfacet::a3Microfacet(float alphaX, float alphaY, a3MicrofacetType type)
    :alphaX(alphaX), alphaY(alphaY), type(type)
{

}

float a3Microfacet::D(const t3Vector3f & m) const
{
    if(a3CosTheta(m) <= 0)
        return 0.0f;

    float result = 0.0f;
    switch(type)
    {
    case Beckmann:
    {
        //float cosTheta2 = a3Cos2Theta(m);
        //float beckmannExponent = ((m.z*m.z) / (alphaX * alphaX)
        //                          + (m.x*m.x) / (alphaY * alphaY)) / cosTheta2;

        //result = t3Math::exp(-beckmannExponent) / (PI * alphaX * alphaY * cosTheta2 * cosTheta2);

        float tan2Theta = a3Tan2Theta(m);
        if(std::isinf(tan2Theta)) 
            return 0.;

        float cos4Theta = a3Cos2Theta(m) * a3Cos2Theta(m);
        return std::exp(-tan2Theta * (a3SphericalCos2Phi(m) / (alphaX * alphaX) +
                                      a3SphericalSin2Phi(m) / (alphaY * alphaY))) /
                                      (PI * alphaX * alphaY * cos4Theta);

        break;
    }
    case GGX:
    case Phong:
        break;
    }

    return result;
}

float a3Microfacet::smithG1(const t3Vector3f & v, const t3Vector3f & m) const
{
    /* Ensure consistent orientation (can't see the back
		   of the microfacet from the front and vice versa) */
    if(v.dot(m) * a3CosTheta(v) <= 0)
        return 0.0f;

    // Perpendicular incidence -- no shadowing/masking
    float tanTheta = t3Math::Abs(a3TanTheta(v));
    if(tanTheta == 0.0f)
        return 1.0f;

    float alpha = 0.0f;
    {
        float invSinTheta2 = 1 / a3Sin2Theta(v);

        if(isIsotropic() || invSinTheta2 <= 0)
            alpha = alphaX;

        float cosPhi2 = v.x * v.x * invSinTheta2;
        float sinPhi2 = v.y * v.y * invSinTheta2;

        alpha = std::sqrt(cosPhi2 * alphaX * alphaY + sinPhi2 * alphaX * alphaY);
    }

    switch(type)
    {
    case Beckmann:
    {
        float a = 1.0f / (alpha * tanTheta);
        if(a >= 1.6f)
            return 1.0f;

        // Use a fast and accurate (<0.35% rel. error) rational approximation to the shadowing-masking function
        float aSqr = a*a;
        return (3.535f * a + 2.181f * aSqr) 
            / (1.0f + 2.276f * a + 2.577f * aSqr);
    }

    case GGX:
    case Phong:
        return 0.0f;
    }
}

bool a3Microfacet::isAnisotropic() const
{
    return alphaX != alphaY;
}

bool a3Microfacet::isIsotropic() const
{
    return alphaX == alphaY;
}

float a3Microfacet::G(const t3Vector3f & wi, const t3Vector3f & wo, const t3Vector3f & m) const
{
    return smithG1(wi, m) * smithG1(wo, m);
}

t3Vector3f a3Microfacet::sample(const t3Vector3f & wi, float& pdf) const
{
    // The azimuthal component is always selected uniformly regardless of the distribution
    float cosThetaM = 0.0f;
    float sinPhiM, cosPhiM;
    float alphaSqr;
    switch(type)
    {
    case Beckmann:
    {
        float sampleX = a3Random::randomFloat(), sampleY = a3Random::randomFloat();

        /* Beckmann distribution function for Gaussian random surfaces */
        if(isIsotropic())
        {
            /* Sample phi component (isotropic case) */
            sinPhiM = t3Math::sinRad((2.0f * PI) * sampleY);
            cosPhiM = t3Math::cosRad((2.0f * PI) * sampleY);

            alphaSqr = alphaX * alphaX;
        }
        else
        {
            /* Sample phi component (anisotropic case) */
            float phiM = t3Math::atanRad(alphaY / alphaX * t3Math::atanRad(PI + 2 * PI*sampleY)) 
                + PI * t3Math::floor(2 * sampleY + 0.5f);

            sinPhiM = t3Math::sinRad(phiM);
            cosPhiM = t3Math::cosRad(phiM);

            float cosSc = cosPhiM / alphaX, sinSc = sinPhiM / alphaY;
            alphaSqr = 1.0f / (cosSc*cosSc + sinSc*sinSc);
        }

        /* Sample theta component */
        float tanThetaMSqr = alphaSqr * -t3Math::log(1.0f - sampleX);
        cosThetaM = 1.0f / std::sqrt(1.0f + tanThetaMSqr);

        /* Compute probability density of the sampled position */
        pdf = (1.0f - sampleX) / (PI*alphaX*alphaY*cosThetaM*cosThetaM*cosThetaM);
    }
    case GGX:
    case Phong:
        break;
    }

    /* Prevent potential numerical issues in other stages of the model */
    if(pdf < 1e-20f)
        pdf = 0;

    float sinThetaM = t3Math::sqrt(t3Math::Max(0.0f, 1.0f - cosThetaM*cosThetaM));

    return t3Vector3f(sinThetaM * cosPhiM, sinThetaM * sinPhiM, cosThetaM);
}

float a3Microfacet::pdf(const t3Vector3f & wi, const t3Vector3f & m) const
{
    return D(m) * a3CosTheta(m);
}
