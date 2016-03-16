#ifndef A3_DISK_H
#define A3_DISK_H

#include <shapes/a3Shape.h>

class a3Disk :public a3Shape
{
public:
    a3Disk(const t3Vector3f& center, const float radius, const t3Vector3f& normal);

    virtual float intersect(const a3Ray& ray) const;

    //--! u1 u2参数未开放
    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint) const;

    // 圆盘上中点及其朝向
    t3Vector3f center, normal;
    
    float radius;
};

#endif