#include <shapes/a3InfinitePlane.h>
#include <core/a3Ray.h>
#include <core/a3Record.h>

a3InfinitePlane::a3InfinitePlane(const t3Matrix4x4 & objectToWorld, const t3Matrix4x4 & worldToObject)
    : a3Shape(objectToWorld, worldToObject)
{
    name = "a3InfinitePlane";
}

a3InfinitePlane::~a3InfinitePlane()
{
}

bool a3InfinitePlane::intersect(const a3Ray & _ray, float * t, float * u, float * v) const
{
    a3Ray ray = _ray;
    ray.transform(worldToObject);

    //--!See https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection for detail
    t3Vector3f normal(0, 1, 0);

    // paralle or not
    double denominator = normal.dot(ray.direction), tHit = 0.0f;

    if(t3Math::Abs(denominator) > A3_EPSILON_FLOAT)
    {
        tHit = (-ray.origin).dot(normal) / denominator;

        if(tHit < ray.minT || tHit > ray.maxT)
            return false;

        *t = tHit;

        return true;
    }

    return false;
}

float a3InfinitePlane::area() const
{
    return A3_INFINITY;
}

void a3InfinitePlane::sample(a3ShapeSamplingRecord & sRec) const
{
    // can't be area light
    sRec.p = t3Vector3f::zero();
    sRec.pdf = pdf(sRec);
    sRec.normal = getNormal(t3Vector3f::zero(), 0, 0);
}

float a3InfinitePlane::pdf(const a3ShapeSamplingRecord & sRec) const
{
    // Not physically based
    return 0.0f;
}

t3Vector3f a3InfinitePlane::getNormal(const t3Vector3f & hitPoint, float u, float v) const
{
    return objectToWorld * t3Vector3f(0, 1, 0);
}
