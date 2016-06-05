#include <lights/a3AreaLight.h>
#include <shapes/a3Shape.h>
#include "a3AreaLight.h"

a3AreaLight::a3AreaLight(const a3Spectrum& emission)
    :emission(emission), a3Light()
{

}

a3AreaLight::~a3AreaLight()
{

}

a3Spectrum a3AreaLight::sampleL(t3Vector3f& wo, const t3Vector3f& p, float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const
{
    // 不做安全检查
    t3Vector3f normal;
    t3Vector3f samplePoint = shapes.sample(sample, &normal);

    // ShapeSet获取采样点samplePoint
    wo = (p - samplePoint).getNormalized();

    *pdf = shapes.pdf(p, -wo);

    vis.setSegment(p, A3_RAY_BIAS, samplePoint, 0.f);

    // 采样点与待求点连线与法线夹角
    float costheta = wo.dot(normal);

    return costheta > 0.0f ? emission : a3Spectrum::zero();
}

float a3AreaLight::pdf(const t3Vector3f&p, const t3Vector3f& wi) const
{
    return shapes.pdf(p, wi);
}

bool a3AreaLight::isDiracDistribution() const
{
    return false;
}

void a3AreaLight::addShape(a3Shape* shape)
{
    shapes.addShape(shape);
}

a3Spectrum a3AreaLight::getEmission() const
{
    return emission;
}

