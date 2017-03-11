#ifndef A3_POINTLIGHT_H
#define A3_POINTLIGHT_H

#include <lights/a3Light.h>

class a3PointLight:public a3Light
{
public:
    a3PointLight(const t3Vector3f& position, const a3Spectrum& intensity);

    a3PointLight(const t3Matrix4x4& lightToWorld, const a3Spectrum& intensity);

    virtual a3Spectrum sampleL(t3Vector3f& wo, const t3Vector3f& p, 
                               float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const;

    virtual bool isDiracDistribution() const;

    virtual float pdf(const t3Vector3f&p, const t3Vector3f& wi) const;

    t3Vector3f position;

    a3Spectrum intensity;
};

#endif