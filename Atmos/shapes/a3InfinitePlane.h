#ifndef A3_INFINITE_PLANE_H
#define A3_INFINITE_PLANE_H

#include <shapes/a3Triangle.h>
#include <t3Math/core/t3Matri4x4.h>

// 单面
class a3InfinitePlane :public a3Shape
{
public:
    a3InfinitePlane(const t3Vector3f& p, const t3Vector3f& normal);

    void set(const t3Vector3f& p, const t3Vector3f& normal);

    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v, float* vtu, float* vtv) const;

    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;

    virtual float area() const;

    virtual t3Vector3f sample(const a3LightSample& sample) const;

    // 平面上一点及其朝向
    t3Vector3f p, normal;
};

#endif