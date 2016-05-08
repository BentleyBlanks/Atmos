#ifndef A3_BVH_H
#define A3_BVH_H

#include <accelerators/a3PrimitiveSet.h>

class a3BVH : public a3PrimitiveSet
{
public:
    virtual bool intersect(const a3Ray& ray, a3Intersection* intersection) const override;

    virtual bool intersect(const a3Ray& ray) const override;
};

#endif
