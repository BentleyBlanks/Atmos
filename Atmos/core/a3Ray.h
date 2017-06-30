#ifndef A3_RAY_H
#define A3_RAY_H

#include <core/a3Settings.h>
#include <t3Math/core/t3Vector3.h>

class a3Ray
{
public:
    a3Ray() : minT(A3_RAY_BIAS), maxT(A3_INFINITY), depth(0){}

	a3Ray(const t3Vector3f& origin, const t3Vector3f& direction, float start = A3_RAY_BIAS, float end = A3_INFINITY, int depth = 0) 
        : origin(origin), direction(direction), minT(start), maxT(end), depth(depth){}

    a3Ray(const t3Vector3f& origin, const t3Vector3f& direction, const a3Ray& parent, float start = A3_RAY_BIAS, float end = A3_INFINITY)
        : origin(origin), direction(direction), minT(start), maxT(end), depth(parent.depth + 1){}

    a3Ray(const a3Ray& ray) 
        : origin(ray.origin), direction(ray.direction), minT(ray.minT), maxT(ray.maxT), depth(ray.depth), time(ray.time){}

    void set(const t3Vector3f& origin, const t3Vector3f& direction)
    {
        this->origin = origin;
        this->direction = direction;
    }

	t3Vector3f operator()(float t) const
	{
		return origin + direction * t;
	}

    void print() const
    {
#ifdef _DEBUG
        origin.print("a3Ray.origin");
        direction.print("a3Ray.direction");
#endif
    }

	t3Vector3f origin, direction;

	float minT, maxT;

	int depth;

    float time;
};

#endif