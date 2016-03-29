#include <shapes/a3Plane.h>
#include <core./a3Ray.h>

a3Plane::a3Plane(const t3Vector3f& p, const t3Vector3f& normal) :p(p), normal(normal.getNormalized()), a3Shape()
{

}

bool a3Plane::intersect(const a3Ray& ray, float* t) const
{
    // �涨�����߷������ʱ�ɼ� �����Ҫ��תnormal
    // �ж�Բ����ֱ��ƽ�й�ϵ
    t3Vector3d _normal(-normal), direction(ray.direction);

    double denominator = (_normal).dot(direction), tHit = 0.0;

    // ����Զƽ�治����˫��
    //denominator = t3Math::Abs(denominator);

    if(denominator > A3_TOLERANCE_DOUBLE)
    {
        t3Vector3d dir(p - ray.origin);

        tHit = (dir.dot(_normal) / denominator);

        if(tHit > ray.minT && tHit < ray.maxT)
        {
            *t = tHit;
            return true;
        }
    }

    return false;
}

t3Vector3f a3Plane::getNormal(const t3Vector3f& hitPoint) const
{
    return normal;
}
