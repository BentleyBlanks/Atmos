#include <shapes/a3Triangle.h>
#include <core/a3Ray.h>

a3Triangle::a3Triangle() :v0(t3Vector3f::zero()), v1(t3Vector3f::zero()), v2(t3Vector3f::zero()), a3Shape()
{

}

a3Triangle::a3Triangle(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2) :v0(v0), v1(v1), v2(v2), a3Shape()
{

}

float a3Triangle::intersect(const a3Ray& ray) const
{
    float t, u, v;

    t3Vector3f v0v1 = v1 - v0;
    t3Vector3f v0v2 = v2 - v0;
    t3Vector3f pvec = ray.direction.getCrossed(v0v2);
    float det = v0v1.dot(pvec);

    // 右手坐标系 三角形唯有在行列式>0时才可见
    if(det < A3_TOLERANCE_DOUBLE) return false;
    float invDet = 1.0f / det;

    // 克莱姆法则计算[t, u, v]三分量
    t3Vector3f tvec = ray.origin - v0;
    u = tvec.dot(pvec) * invDet;
    if(u < 0 || u > 1) return false;

    t3Vector3f qvec = tvec.getCrossed(v0v1);
    v = ray.direction.dot(qvec) * invDet;
    if(v < 0 || u + v > 1) return false;

    // 可直接返回
    t = v0v2.dot(qvec) * invDet;

    return t;
}

t3Vector3f a3Triangle::getNormal(const t3Vector3f& vector) const
{
    //// 左手坐标系
    //t3Vector3f temp = (v1 - v0).getCrossed(v2 - v0);
    //return temp;
    return n0;
}
