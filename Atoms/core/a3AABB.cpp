#include <core/a3AABB.h>
#include <core/a3Ray.h>
#include <t3Math/core/t3Math.h>

// --!Copyied From [PBRT grometry.h]
a3AABB::a3AABB() : min(A3_INFINITY), max(-A3_INFINITY)
{

}

a3AABB::a3AABB(const t3Vector3f& p):min(p), max(p)
{

}

a3AABB::a3AABB(const t3Vector3f& p0, const t3Vector3f& p1):min(p0), max(p1)
{

}

bool a3AABB::overlaps(const a3AABB& b) const
{
    bool x = (max.x >= b.min.x) && (min.x <= b.max.x);
    bool y = (max.y >= b.min.y) && (min.y <= b.max.y);
    bool z = (max.z >= b.min.z) && (min.z <= b.max.z);
    return (x && y && z);
}

bool a3AABB::inside(const t3Vector3f& point) const
{
    return (point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z);
}

void a3AABB::expand(float delta)
{
    min -= t3Vector3f(delta, delta, delta);
    max += t3Vector3f(delta, delta, delta);
}

float a3AABB::surfaceArea() const
{
    t3Vector3f d = max - min;
    return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

float a3AABB::volume() const
{
    t3Vector3f d = max - min;
    return d.x * d.y * d.z;
}

int a3AABB::maxExtentIndex() const
{
    t3Vector3f diagonal = max - min;
    if(diagonal.x > diagonal.y && diagonal.x > diagonal.z)
        return 0;
    else if(diagonal.y > diagonal.z)
        return 1;
    else
        return 2;
}

t3Vector3f a3AABB::interpolate(float tx, float ty, float tz)
{
    return t3Vector3f(t3Math::interpolate(min.x, max.x, tx),
                      t3Math::interpolate(min.y, max.y, ty),
                      t3Math::interpolate(min.z, max.z, tz));
}

void a3AABB::boundingSphere(t3Vector3f* center, float* radius) const
{
    *center = .5f * min + .5f * max;
    *radius = inside(*center) ? (*center - max).length() : 0.f;
}

bool a3AABB::intersect(const a3Ray& ray, float *hitt0, float *hitt1) const
{
    float t0 = ray.minT, t1 = ray.maxT;
    for(int i = 0; i < 3; ++i)
    {
        // Update interval for _i_th bounding box slab
        float invRayDir = 1.f / ray.direction[i];
        float tNear = (min[i] - ray.origin[i]) * invRayDir;
        float tFar = (max[i] - ray.origin[i]) * invRayDir;

        // Update parametric interval from slab intersection $t$s
        if(tNear > tFar) t3Math::swap(tNear, tFar);

        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;
        if(t0 > t1) return false;
    }
    if(hitt0) *hitt0 = t0;
    if(hitt1) *hitt1 = t1;
    return true;
}

a3AABB a3AABB::calUnion(const a3AABB& b, const t3Vector3f& p)
{
    a3AABB ret = b;
    ret.min.x = t3Math::Min(b.min.x, p.x);
    ret.min.y = t3Math::Min(b.min.y, p.y);
    ret.min.z = t3Math::Min(b.min.z, p.z);
    ret.max.x = t3Math::Max(b.max.x, p.x);
    ret.max.y = t3Math::Max(b.max.y, p.y);
    ret.max.z = t3Math::Max(b.max.z, p.z);
    return ret;
}

a3AABB a3AABB::calUnion(const a3AABB& b, const a3AABB& b2)
{
    a3AABB ret;
    ret.min.x = t3Math::Min(b.min.x, b2.min.x);
    ret.min.y = t3Math::Min(b.min.y, b2.min.y);
    ret.min.z = t3Math::Min(b.min.z, b2.min.z);
    ret.max.x = t3Math::Max(b.max.x, b2.max.x);
    ret.max.y = t3Math::Max(b.max.y, b2.max.y);
    ret.max.z = t3Math::Max(b.max.z, b2.max.z);
    return ret;
}

void a3AABB::print()
{
#ifdef _DEBUG
    min.print();
    max.print();
#endif
}

void a3AABB::print(std::string name)
{
#ifdef _DEBUG
    min.print(name + ".min");
    max.print(name + ".max");
#endif
}
