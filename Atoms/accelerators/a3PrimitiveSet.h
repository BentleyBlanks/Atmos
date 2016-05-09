#ifndef A3_PRIMITIVESET_H
#define A3_PRIMITIVESET_H

#include <core/a3Settings.h>

class a3Shape;
class a3Intersection;
class a3Ray;

class a3PrimitiveSet
{
public:
    virtual bool intersect(const a3Ray& ray, a3Intersection* intersection) const = 0;

    virtual bool intersect(const a3Ray& ray) const = 0;

    bool addShape(a3Shape* shape);

    // 所有shape对象
    std::vector<a3Shape*> primitives;
};

#endif