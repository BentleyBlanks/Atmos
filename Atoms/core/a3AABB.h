#ifndef A3_AABB_H
#define A3_AABB_H

#include <t3Math/core/t3Vector3.h>
#include <core/a3Settings.h>

class a3Ray;

class a3AABB
{
public:
    a3AABB();
    a3AABB(const t3Vector3f& p);
    a3AABB(const t3Vector3f& p0, const t3Vector3f& p1);

    bool overlaps(const a3AABB& bbox) const;

    bool inside(const t3Vector3f& point) const;

    void expand(float delta);

    float surfaceArea() const;

    float volume() const;

    // 返回当前BBox边长最大的轴的序号
    int maxExtentIndex() const;

    // 线性插值
    t3Vector3f interpolate(float tx, float ty, float tz);

    void boundingSphere(t3Vector3f* center, float* radius) const;

    // 与Accelerator中暂不启用
    bool intersect(const a3Ray& ray, float *hitT0, float *hitT1) const;

    static a3AABB calUnion(const a3AABB& box, const t3Vector3f& p);

    static a3AABB calUnion(const a3AABB& a, const a3AABB& b);

    // x:0 y:1 z:2
    inline t3Vector3f& operator[](int i);

    inline const t3Vector3f& operator[](int i)const;

    inline bool operator==(const a3AABB& bbox) const;

    inline bool operator!=(const a3AABB& bbox) const;

    void print();

    void print(std::string name);

    t3Vector3f min, max;
};

#include <core/a3AABB.inl>

#endif
