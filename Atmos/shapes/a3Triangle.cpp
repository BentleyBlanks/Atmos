#include <shapes/a3Triangle.h>
#include <core/a3Ray.h>
#include <core/a3Record.h>
#include <core/a3Warp.h>
#include <core/random/a3Random.h>
#include <t3Math/core/t3Math.h>

a3Triangle::a3Triangle() :v0(t3Vector3f::zero()), v1(t3Vector3f::zero()), v2(t3Vector3f::zero()), bEnableBackfaceCulling(false), a3Shape(t3Matrix4x4())
{
    name = "a3Triangle";

    aabb.set(t3Vector3f(t3Math::Min(v0.x, v1.x, v2.x),
                        t3Math::Min(v0.y, v1.y, v2.y),
                        t3Math::Min(v0.z, v1.z, v2.z)),
             t3Vector3f(t3Math::Max(v0.x, v1.x, v2.x),
                        t3Math::Max(v0.y, v1.y, v2.y),
                        t3Math::Max(v0.z, v1.z, v2.z)));
}

a3Triangle::a3Triangle(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2) : v0(v0), v1(v1), v2(v2), bEnableBackfaceCulling(false), a3Shape(t3Matrix4x4())
{
    name = "a3Triangle";

    aabb.set(t3Vector3f(t3Math::Min(v0.x, v1.x, v2.x),
                        t3Math::Min(v0.y, v1.y, v2.y),
                        t3Math::Min(v0.z, v1.z, v2.z)),
             t3Vector3f(t3Math::Max(v0.x, v1.x, v2.x),
                        t3Math::Max(v0.y, v1.y, v2.y),
                        t3Math::Max(v0.z, v1.z, v2.z)));
}

a3Triangle::~a3Triangle()
{
}

void a3Triangle::set(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2)
{
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;

    aabb.set(t3Vector3f(t3Math::Min(v0.x, v1.x, v2.x),
                        t3Math::Min(v0.y, v1.y, v2.y),
                        t3Math::Min(v0.z, v1.z, v2.z)),
             t3Vector3f(t3Math::Max(v0.x, v1.x, v2.x),
                        t3Math::Max(v0.y, v1.y, v2.y),
                        t3Math::Max(v0.z, v1.z, v2.z)));
}

void a3Triangle::setVertexNormal(t3Vector3f n0, t3Vector3f n1, t3Vector3f n2)
{
    this->n0 = n0;
    this->n1 = n1;
    this->n2 = n2;
}

void a3Triangle::setTextureCoordinate(t3Vector3f vt0, t3Vector3f vt1, t3Vector3f vt2)
{
    this->vt0 = vt0;
    this->vt1 = vt1;
    this->vt2 = vt2;
}

bool a3Triangle::intersect(const a3Ray& ray, float* t, float* _u, float* _v) const
{
    double tHit, u, v;

    t3Vector3d dir(ray.direction);
    t3Vector3d v0v1(v1 - v0);
    t3Vector3d v0v2(v2 - v0);
    t3Vector3d pvec = dir.getCrossed(v0v2);
    double det = v0v1.dot(pvec);

    // 右手坐标系 三角形唯有在行列式>0时才可见
    if(bEnableBackfaceCulling)
        if(det < A3_EPSILON_DOUBLE)
            return false;
        else
            if(t3Math::Abs(det) < A3_EPSILON_DOUBLE)
                return false;

    double invDet = 1.0 / det;

    // 克莱姆法则计算[t, u, v]三分量
    t3Vector3d tvec(ray.origin - v0);
    u = tvec.dot(pvec) * invDet;
    if(u < 0.0 || u > 1.0) return false;

    t3Vector3d qvec = tvec.getCrossed(v0v1);
    v = dir.dot(qvec) * invDet;
    if(v < 0.0 || u + v > 1.0) return false;

    // 可直接返回
    tHit = v0v2.dot(qvec) * invDet;

    if(tHit > ray.minT && tHit < ray.maxT)
    {
        *t = tHit;
        *_u = u;
        *_v = v;

        //if(bCalTextureCoordinate)
        //{
        //    // --!纹理坐标
        //    t3Vector3f vt = (1 - u - v) * vt0 + u * vt1 + v * vt2;
        //    *vtu = vt.x;
        //    *vtv = vt.y;
        //}

        return true;
    }

    return false;
}

float a3Triangle::area() const
{
    // 二分之一的平行四边形面积
    return 0.5f * (v1 - v0).cross(v2 - v0).length();
}

void a3Triangle::sample(a3ShapeSamplingRecord & sRec) const
{
    a3Random r;
    t3Vector2f b = a3UniformSampleTriangle(r.randomFloat(), r.randomFloat());

    sRec.p = b.x * v0 + b.y * v1 + (1.0 - b.x - b.y) * v2;
    sRec.normal = getNormal(t3Vector3f::zero(), b.x, b.y).getNormalized();
    sRec.pdf = 1 / area();
}

t3Vector3f a3Triangle::getNormal(const t3Vector3f & hitPoint, float u, float v) const
{
    t3Vector3f normal;

    if(bUseFaceNormal)
        // 左手坐标系
        normal.set((v1 - v0).getCrossed(v2 - v0).getNormalized());
    else
        normal.set((1 - u - v) * n0 + u * n1 + v * n2);

    return normal;
}