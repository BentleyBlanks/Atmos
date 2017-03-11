#ifndef A3_SPOTLIGHT_H
#define A3_SPOTLIGHT_H

#include <lights/a3Light.h>

class a3SpotLight : public a3Light
{
public:
    // 内角, 从内角开始产生半影的角度
    a3SpotLight(const t3Vector3f& position, const t3Vector3f& direction, const a3Spectrum& intensity, float coneAngle, float coneDeltaAngle);

    virtual a3Spectrum sampleL(t3Vector3f& wo, const t3Vector3f& p, float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const;

    virtual float pdf(const t3Vector3f&p, const t3Vector3f& wi) const;

    virtual bool isDiracDistribution() const;

    // 根据聚光灯可见区域分布计算强度比例
    float calculateFalloff(const t3Vector3f& wo) const;

    // 强度
    a3Spectrum intensity;

    // 位置 / 朝向
    t3Vector3f position, direction;

    // 圆锥与中心线夹角(弧度)
    float cosConeAngle;

    // 开始产生半影的角度(弧度)
    float cosFalloffStart;
};

#endif