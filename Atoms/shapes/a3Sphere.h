#ifndef A3_SPHERE_H
#define A3_SPHERE_H

#include <shapes/a3Shape.h>

class a3Sphere : public a3Shape
{
public:
	a3Sphere(const t3Vector3f& center, const float radius = 0.0f);

	bool intersect(const a3Ray& ray, float* t) const;

    //t3Vector3f getNormal(const t3Vector3f& vector) const;

    //--! u1 u2参数未开放
    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint) const;

    virtual void print() const;

	t3Vector3f center;

	float radius;
};

#endif