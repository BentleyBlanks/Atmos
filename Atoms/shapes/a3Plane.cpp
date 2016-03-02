#include <shapes/a3Plane.h>
#include <core./a3Ray.h>

a3Plane::a3Plane(const t3Vector3f& p, const t3Vector3f& normal) :p(p), normal(normal.getNormalized()), a3Shape()
{

}

float a3Plane::intersect(const a3Ray& ray) const
{
    // �ж�Բ����ֱ��ƽ�й�ϵ
    float denominator = normal * ray.direction, t = 0.0f;

    // ����Զƽ�治����˫��
    //denominator = t3Math::Abs(denominator);

    if(denominator > T3MATH_DEFAULT_EPSILON)
    {
        t3Vector3f dir = p - ray.origin;

        return (dir * normal / denominator);
    }

    return 0.0f;
}

t3Vector3f a3Plane::getNormal(const t3Vector3f& vector) const
{
    return normal;
}

