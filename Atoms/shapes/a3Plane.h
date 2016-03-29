#ifndef A3_PLANE_H
#define A3_PLANE_H

#include <shapes/a3Shape.h>

// 单面
class a3Plane:public a3Shape
{
public:
    a3Plane(const t3Vector3f& p, const t3Vector3f& normal);

    virtual bool intersect(const a3Ray& ray, float* t) const;

    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint) const;

    // 平面上一点及其朝向
    t3Vector3f p, normal;
};

#endif