#ifndef A3_SPHERE_H
#define A3_SPHERE_H

#include <shapes/a3Shape.h>

class a3Sphere : public a3Shape
{
public:
	a3Sphere(const t3Vector3f& center, const float radius = 0.0f);

	float intersect(const a3Ray& ray) const;

	t3Vector3f normal(const t3Vector3f& vector) const;

    virtual void print() const;

	t3Vector3f center;

	float radius;
};

#endif