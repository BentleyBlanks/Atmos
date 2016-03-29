#include <shapes/a3Sphere.h>
#include <core/a3Ray.h>
#include <core/a3Warp.h>

a3Sphere::a3Sphere(const t3Vector3f& center, const float radius) :center(center), radius(radius), a3Shape()
{

}

bool a3Sphere::intersect(const a3Ray& ray, float* t) const
{
    t3Vector3d o(ray.origin.x - center.x, ray.origin.y - center.y, ray.origin.z - center.z);
    t3Vector3d d(ray.direction.x, ray.direction.y, ray.direction.z);

	// direction为单位向量，长度为1
	double A = d.lengthSquared();
    double B = 2 * o.dot(d);
    double C = o.dot(o) - radius * radius;

    double nearT, farT;
    if(!a3SolveQuadraticDouble(A, B, C, &nearT, &farT))
        return false;

    // 任何一者不满足就剔除
    if(nearT > ray.maxT || farT < ray.minT)
        return false;

    *t = nearT;
    if(nearT < ray.minT)
    {
        *t = farT;
        if(*t > ray.maxT)
            return false;
    }

    return true;
}

t3Vector3f a3Sphere::getNormal(const t3Vector3f& hitPoint) const
{
    return (hitPoint - center).normalize();
}

void a3Sphere::print() const
{
#ifdef _DEBUG
    center.print("a3Sphere:center");

    std::cout << "a3Sphere:radius:" << radius << std::endl;

    a3Shape::print();
#endif
}
