#include <shapes/a3Sphere.h>
#include <core/a3Ray.h>
#include <core/a3Warp.h>
#include <core/a3Record.h>
#include <core/random/a3Random.h>

a3Sphere::a3Sphere(const t3Matrix4x4 & shapeToWorld, const float radius)
    :radius(radius), a3Shape(shapeToWorld)
{
    name = "a3Sphere";

    center = shapeToWorld * t3Vector3f(0, 0, 0);

    //aabb.set(t3Vector3f(center - radius), t3Vector3f(center + radius));
}

a3Sphere::~a3Sphere()
{
}

bool a3Sphere::intersect(const a3Ray & ray, float * t, float * u, float * v) const
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

float a3Sphere::area() const
{
    return 4 * T3MATH_PI * radius * radius;
}

void a3Sphere::sample(a3ShapeSamplingRecord & sRec) const
{
    static a3Random r;
    // uniform sampling the disk
    t3Vector3f sampledP = a3UniformSampleSphere(r.randomFloat(), r.randomFloat()) * radius;

    sRec.p = shapeToWorld * sampledP;
    sRec.pdf = 1 / area();
    sRec.normal = getNormal(sRec.p, 0, 0);
}

t3Vector3f a3Sphere::getNormal(const t3Vector3f & hitPoint, float u, float v) const
{
    return (hitPoint - center).getNormalized();
}

t3Matrix4x4 a3Sphere::getShadeToWorld(const a3IntersectRecord & its) const
{
    t3Vector3f normal = getNormal(its.p, 0, 0), x, z;
    a3OrthonomalSystem(normal, x, z);

    return t3Matrix4x4(x.x, normal.x, z.x, 0,
                       x.y, normal.y, z.y, 0,
                       x.z, normal.z, z.z, 0,
                       0, 0, 0, 1);
}
