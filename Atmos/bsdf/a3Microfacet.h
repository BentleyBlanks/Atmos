#pragma once

#include <t3Math/core/t3Vector3.h>

class a3Microfacet
{
public:
    enum a3MicrofacetType
    {
        Beckmann = 0,
        GGX = 1,
        Phong = 2
    };

    a3Microfacet(float alphaX, float alphaY, a3MicrofacetType type = Beckmann);

    // evaluate the microfacet distribution function(D(wh))
    float D(const t3Vector3f& m) const;

    // Separable shadow - masking function based on Smith's one - dimensional masking model
    float G(const t3Vector3f &wi, const t3Vector3f &wo, const t3Vector3f &m) const;

    // Draw a sample from the microfacet normal distribution and return the associated probability density
    t3Vector3f sample(const t3Vector3f& wi, float& pdf) const;

    // Returns the density function associated with the sample function
    float pdf(const t3Vector3f& wi, const t3Vector3f& m) const;

    // Smith's shadowing-masking function G1 for each of the supported microfacet distributions
    float smithG1(const t3Vector3f &v, const t3Vector3f &m) const;
 
    // Is this an anisotropic microfacet distribution?
    bool isAnisotropic() const;

    // Is this an anisotropic microfacet distribution?
    bool isIsotropic() const;

    // Supported distribution types
    a3MicrofacetType type;

    float alphaX, alphaY;
};