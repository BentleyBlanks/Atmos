#ifndef A3_RAY_H
#define A3_RAY_H

#include <core/a3Settings.h>
#include <t3Math/core/t3Vector3.h>

class a3Ray
{
public:
	a3Ray() : mint(0.0f), maxt(0.0f), depth(0){}

	a3Ray(const t3Vector3f& origin, const t3Vector3f& direction, float start, float end = A3_INFINITY, int depth = 0) 
        : origin(origin), direction(direction), mint(start), maxt(end), depth(depth){}

	a3Ray(const t3Vector3f& origin, const t3Vector3f& direction, const a3Ray& parent, float start, float end = A3_INFINITY) 
        : origin(origin), direction(direction), mint(start), maxt(end), depth(parent.depth + 1){}

	t3Vector3f operator()(float t) const
	{
		return origin + direction * t;
	}

    void print() const
    {
#ifdef _DEBUG
        origin.print("a3Ray.origin");
        direction.print("ray.direction");
#endif
    }

	t3Vector3f origin, direction;

	float mint, maxt;

	int depth;

};

#endif