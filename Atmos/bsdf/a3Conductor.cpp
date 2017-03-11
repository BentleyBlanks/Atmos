#include <bsdf/a3Conductor.h>

a3Conductor::a3Conductor(a3Spectrum R) :R(R)
{
    name = "a3Conductor";
}

a3Spectrum a3Conductor::sample(const t3Vector3f& wi, t3Vector3f& wo, float* pdf, const a3Intersection& its) const
{
    t3Vector3f normal = its.getNormal();

    wo = (wi - 2 * normal.dot(wi) * normal).normalize();

    *pdf = 1.0f;

    // delta分布无需计算cosTheta, 先除则在外部累乘时抵消
    return  getColor(its) * R / wo.dot(normal);
}

a3Spectrum a3Conductor::eval(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const
{
    // delta分布在给定出射方向上几率为0
    return a3Spectrum(0.0f);
}

bool a3Conductor::isDiracDistribution() const
{
    return true;
}

float a3Conductor::pdf(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const
{
    return 0.0f;
}

//bool a3Conductor::isReflection() const
//{
//    return true;
//}
//
//