#include <shapes/a3Plane.h>
#include <core./a3Ray.h>

a3Plane::a3Plane(const t3Vector3f& p, const t3Vector3f& normal) :p(p), normal(normal.getNormalized()), a3Shape()
{

}

float a3Plane::intersect(const a3Ray& ray) const
{
    // �涨�����߷������ʱ�ɼ� �����Ҫ��תnormal
    // �ж�Բ����ֱ��ƽ�й�ϵ
    float denominator = (-normal).dot(ray.direction), t = 0.0f;

    // ����Զƽ�治����˫��
    //denominator = t3Math::Abs(denominator);

    if(denominator > A3_TOLERANCE_FLOAT)
    {
        t3Vector3f dir = p - ray.origin;

        t = (dir.dot(-normal) / denominator);

        if(t > A3_TOLERANCE_FLOAT)
            return t;
    }

    return 0.0f;
}

t3Vector3f a3Plane::getNormal(const t3Vector3f& hitPoint) const
{
    return normal;
}
