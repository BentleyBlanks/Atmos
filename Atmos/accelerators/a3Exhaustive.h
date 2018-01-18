#ifndef A3_EXHAUSTIVE_H
#define A3_EXHAUSTIVE_H

#include <accelerators/a3PrimitiveSet.h>

class a3Exhaustive :public a3PrimitiveSet
{
public:
    virtual bool intersect(const a3Ray& ray, a3IntersectRecord* intersection) const override;

    virtual bool intersect(const a3Ray& ray) const override;
};

#endif