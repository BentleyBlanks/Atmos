#ifndef A3_PRIMITIVESET_H
#define A3_PRIMITIVESET_H

#include <core/a3Settings.h>

class a3Shape;
class a3IntersectRecord;
class a3Ray;

class a3PrimitiveSet
{
public:
    virtual ~a3PrimitiveSet();

    virtual bool intersect(const a3Ray& ray, a3IntersectRecord* intersection) const = 0;

    virtual bool intersect(const a3Ray& ray) const = 0;

    bool addShape(a3Shape* shape);

    // 所有shape对象
    std::vector<a3Shape*> primitives;
};

#endif