#include <shapes/a3InfinitePlane.h>
#include <core./a3Ray.h>
#include <core/a3Warp.h>
#include <samples/a3LightSample.h>
#include <core/random/a3Random.h>

a3InfinitePlane::a3InfinitePlane(const t3Vector3f& p, const t3Vector3f& normal)
    :p(p), normal(normal.getNormalized()), a3Shape("a3InfinitePlane")
{
    aabb.set(t3Vector3f(-A3_INFINITY), t3Vector3f(A3_INFINITY));
}


void a3InfinitePlane::set(const t3Vector3f& p, const t3Vector3f& normal)
{
    this->p = p;
    this->normal = normal.getNormalized();

    aabb.set(t3Vector3f(-A3_INFINITY), t3Vector3f(A3_INFINITY));
}

t3Vector3f a3InfinitePlane::sample(const a3LightSample& sample) const
{
    a3Random random;

    return t3Vector3f(random.randomFloat() * A3_INFINITY, random.randomFloat() * A3_INFINITY, random.randomFloat() * A3_INFINITY);
}

float a3InfinitePlane::area() const
{
    return A3_INFINITY;
}

bool a3InfinitePlane::intersect(const a3Ray& ray, float* _t, float* _u, float* _v, float* _vtu, float* _vtv) const
{
    // 规定与射线方向相对时可见 因此需要反转normal
    // 判断圆盘与直线平行关系
    t3Vector3d _normal(-normal), direction(ray.direction);

    double denominator = (_normal).dot(direction), tHit = 0.0;

    // 无限远平面不适用双面
    //denominator = t3Math::Abs(denominator);

    if(denominator > A3_TOLERANCE_DOUBLE)
    {
        t3Vector3d dir(p - ray.origin);

        tHit = (dir.dot(_normal) / denominator);

        if(tHit > ray.minT && tHit < ray.maxT)
        {
            *_t = tHit;
            return true;
        }
    }

    return false;
}

t3Vector3f a3InfinitePlane::getNormal(const t3Vector3f& hitPoint, float u, float v) const
{
    return normal;
}
