#include <shapes/a3Triangle.h>
#include <core/a3Ray.h>

a3Triangle::a3Triangle() :v0(t3Vector3f::zero()), v1(t3Vector3f::zero()), v2(t3Vector3f::zero()), a3Shape()
{

}

a3Triangle::a3Triangle(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2) :v0(v0), v1(v1), v2(v2), a3Shape()
{

}

bool a3Triangle::intersect(const a3Ray& ray, float* t) const
{
    double tHit, u, v;

    t3Vector3d dir(ray.direction);
    t3Vector3d v0v1(v1 - v0);
    t3Vector3d v0v2(v2 - v0);
    t3Vector3d pvec = dir.getCrossed(v0v2);
    double det = v0v1.dot(pvec);

    // ��������ϵ ������Ψ��������ʽ>0ʱ�ſɼ�
    if(det < A3_TOLERANCE_DOUBLE) return false;
    double invDet = 1.0 / det;

    // ����ķ�������[t, u, v]������
    t3Vector3d tvec(ray.origin - v0);
    u = tvec.dot(pvec) * invDet;
    if(u < 0 || u > 1) return false;

    t3Vector3d qvec = tvec.getCrossed(v0v1);
    v = dir.dot(qvec) * invDet;
    if(v < 0 || u + v > 1) return false;

    // ��ֱ�ӷ���
    tHit = v0v2.dot(qvec) * invDet;

    if(tHit > ray.minT && tHit < ray.maxT)
    {
        *t = tHit;
        return true;
    }

    return false;
}

t3Vector3f a3Triangle::getNormal(const t3Vector3f& hitPoint) const
{
    //// ��������ϵ
    //t3Vector3f temp = (v1 - v0).getCrossed(v2 - v0).getNormalized();
    //return temp;

    // �ⲿ��֤hitPoint���������ڲ�
    // --!Ч�ʵ͵���
    //t3Vector3f pv0 = v0 - hitPoint, pv1 = v1 - hitPoint, pv2 = v2 - hitPoint;
    //float S0 = pv0.getCrossed(pv1).length(), S1 = pv1.getCrossed(pv2).length(), S2 = pv2.getCrossed(pv0).length();
    //float S = S0 + S1 + S2;
    //float u = S1 / S, v = S2 / S;

    //return (u * n0 + (1 - u - v)* n1 + v * n2);

    return n0;
}
