#pragma once

#include <shapes/a3Shape.h>
class a3Triangle :public a3Shape
{
public:
    a3Triangle();

    a3Triangle(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2);

    virtual ~a3Triangle();

    // ray intersection test for visibility queries
    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v) const;

    // compute the area
    virtual float area() const;

    // Sample a point on the surface of this shape instance
    virtual void sample(a3ShapeSamplingRecord& sRec) const;

    // return the normal vec from the point be hitted
    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;

    // setting
    void set(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2);

    void setVertexNormal(t3Vector3f n0, t3Vector3f n1, t3Vector3f n2);

    void setTextureCoordinate(t3Vector3f vt0, t3Vector3f vt1, t3Vector3f vt2);

    t3Vector3f v0, v1, v2;

    // 顶点法线
    t3Vector3f n0, n1, n2;

    // 顶点纹理坐标
    t3Vector3f vt0, vt1, vt2;

    // 是否使用单一面片法线
    bool bUseFaceNormal;

    bool bEnableBackfaceCulling;
};