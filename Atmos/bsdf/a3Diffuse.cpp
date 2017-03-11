#include <bsdf/a3Diffuse.h>
#include <core/a3Warp.h>

a3Diffuse::a3Diffuse(a3Spectrum R) :R(R)
{
    name = "a3Diffuse";
}

a3Spectrum a3Diffuse::sample(const t3Vector3f& wi, t3Vector3f& wo, float* pdf, const a3Intersection& its) const
{
    static a3Random random;
    // Diffuse BRDF
    t3Vector3f normal = its.getNormal();

    t3Vector3f rotatedX, rotatedY;
    a3OrthonomalSystem(normal, rotatedX, rotatedY);

    // 均匀半球随机采样
    t3Vector3f sampleDirection = a3Hemisphere(random.randomFloat(), random.randomFloat());
    sampleDirection.normalize();

    // sampleDirection转换到Normal坐标系下
    wo.x = t3Vector3f(rotatedX.x, rotatedY.x, normal.x).dot(sampleDirection);
    wo.y = t3Vector3f(rotatedX.y, rotatedY.y, normal.y).dot(sampleDirection);
    wo.z = t3Vector3f(rotatedX.z, rotatedY.z, normal.z).dot(sampleDirection);

    // cosin weighted
    *pdf = t3Math::Abs(wo.dot(normal)) * T3MATH_INV_PI;

    // [PBR]: Lambertian
    return getColor(its) * R * T3MATH_INV_PI;
}

a3Spectrum a3Diffuse::eval(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const
{
    // [PBR]: Lambertian
    return getColor(its) * R * T3MATH_INV_PI;
}

bool a3Diffuse::isDiracDistribution() const
{
    return false;
}

float a3Diffuse::pdf(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const
{
    // cosin weighted
    return t3Math::Abs(wo.dot(its.getNormal())) * T3MATH_INV_PI;
}

//bool a3Diffuse::isReflection() const
//{
//    return false;
//}
//