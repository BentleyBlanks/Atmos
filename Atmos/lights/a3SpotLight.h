#ifndef A3_SPOTLIGHT_H
#define A3_SPOTLIGHT_H

#include <lights/a3Light.h>

class a3SpotLight : public a3Light
{
public:
    // �ڽ�, ���ڽǿ�ʼ������Ӱ�ĽǶ�
    a3SpotLight(const t3Vector3f& position, const t3Vector3f& direction, const a3Spectrum& intensity, float coneAngle, float coneDeltaAngle);

    virtual a3Spectrum sampleL(t3Vector3f& wo, const t3Vector3f& p, float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const;

    virtual float pdf(const t3Vector3f&p, const t3Vector3f& wi) const;

    virtual bool isDiracDistribution() const;

    // ���ݾ۹�ƿɼ�����ֲ�����ǿ�ȱ���
    float calculateFalloff(const t3Vector3f& wo) const;

    // ǿ��
    a3Spectrum intensity;

    // λ�� / ����
    t3Vector3f position, direction;

    // Բ׶�������߼н�(����)
    float cosConeAngle;

    // ��ʼ������Ӱ�ĽǶ�(����)
    float cosFalloffStart;
};

#endif