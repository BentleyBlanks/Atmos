#include <shapes/a3Disk.h>
#include <core/a3Ray.h>

a3Disk::a3Disk(const t3Vector3f& center, const float radius, const t3Vector3f& normal) :center(center), radius(radius), normal(normal.getNormalized()), a3Shape()
{

}

bool a3Disk::intersect(const a3Ray& ray, float* t) const
{
    t3Vector3d _normal(-normal), direction(ray.direction);

    // 判断圆盘与直线平行关系
    double denominator = _normal.dot(direction), tHit = 0.0f;

    if(denominator > A3_TOLERANCE_DOUBLE)
    {
        t3Vector3d dir(center - ray.origin);

        tHit = dir.dot(_normal) / denominator;

        if(tHit < ray.minT || tHit > ray.maxT)
            return false;

        t3Vector3d p(ray(tHit)), d = p - t3Vector3d(center);

        // 求取平面上相交点p与中心点center距离
        if(d.lengthSquared() <= radius * radius)
        {
            *t = tHit;
            return true;
        }
    }

    return false;
}

t3Vector3f a3Disk::getNormal(const t3Vector3f& hitPoint) const
{
    return normal;
}

