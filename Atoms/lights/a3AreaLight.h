#ifndef A3_AREA_LIGHT_H
#define A3_AREA_LIGHT_H

#include <lights/a3Light.h>
#include <core/a3Spectrum.h>
#include <shapes/a3ShapeSet.h>

// Diffuse Area Light
class a3AreaLight : public a3Light
{
public:
    // 光源上仅采样一次
    a3AreaLight(const a3Spectrum& emission);

    ~a3AreaLight();

    virtual a3Spectrum sampleL(t3Vector3f& wo, const t3Vector3f& p, float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const;

    virtual float pdf(const t3Vector3f&p, const t3Vector3f& wi) const;

    virtual bool isDiracDistribution() const;

    void addShape(a3Shape* shape);

    a3Spectrum getEmission() const;

    // 发光强度
    a3Spectrum emission;

    a3ShapeSet shapes;
};

#endif
