#include <shapes/a3Disk.h>
#include <core/a3Ray.h>

a3Disk::a3Disk(const t3Vector3f& center, const float radius, const t3Vector3f& normal) :center(center), radius(radius), normal(normal.getNormalized()), a3Shape()
{

}

float a3Disk::intersect(const a3Ray& ray) const
{
    // 判断圆盘与直线平行关系
    float denominator = normal.dot(ray.direction), t = 0.0f;


    if(denominator > T3MATH_DEFAULT_EPSILON)
    {
        t3Vector3f dir = center - ray.origin;

        t = dir.dot(normal) / denominator;

        t3Vector3f p = ray(t), d = p - center;

        // 求取平面上相交点p与中心点center距离
        if(d.lengthSquared() <= radius * radius)
            return t;
    }

    return 0.0f;
}

t3Vector3f a3Disk::getNormal(const t3Vector3f& hitPoint) const
{
    return normal;
}

