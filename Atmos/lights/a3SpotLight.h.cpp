#include <lights/a3SpotLight.h>
#include <core/a3Settings.h>
#include <core/log/a3Log.h>

a3SpotLight::a3SpotLight(const t3Vector3f& position, const t3Vector3f& direction, const a3Spectrum& intensity, float coneAngleDeg, float coneDeltaAngleDeg) 
: position(position), 
  direction(direction.getNormalized()), 
  intensity(intensity), 
  cosConeAngle(t3Math::cosDeg(coneAngleDeg)), 
  cosFalloffStart(t3Math::cosDeg(coneAngleDeg - coneDeltaAngleDeg)),
  a3Light("a3SpotLight")
{
    // 暂不启用光源局部坐标系
    //lightToWorld.set();
    //worldToLight = lightToWorld.getInverse();

    if(coneDeltaAngleDeg < 0 || coneAngleDeg < 0 || coneAngleDeg < coneDeltaAngleDeg)
        a3Log::error("a3SpotLight::a3SpotLight() 聚光灯角度参数有误\n");
}

a3Spectrum a3SpotLight::sampleL(t3Vector3f& wo, const t3Vector3f& p, 
                                float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const
{
    t3Vector3f toLight = position - p;

    wo = toLight.getNormalized();

    *pdf = 1.0f;

    vis.setSegment(p, A3_RAY_BIAS, position, 0.0f);

    return intensity * calculateFalloff(-wo) / toLight.lengthSquared();
}

float a3SpotLight::pdf(const t3Vector3f&p, const t3Vector3f& wi) const
{
    return 0.0f;
}

bool a3SpotLight::isDiracDistribution() const
{
    return true;
}

float a3SpotLight::calculateFalloff(const t3Vector3f& wo) const
{
    float cosTheta = wo.dot(direction);

    // 圆锥+半影开外
    if(cosTheta < cosConeAngle)
        return 0.0f;

    // 内角中
    if(cosTheta > cosFalloffStart)
        return 1.0f;

    float ratio = (cosTheta - cosConeAngle) / (cosFalloffStart - cosConeAngle);

    return ratio * ratio * ratio * ratio * ratio;
}
