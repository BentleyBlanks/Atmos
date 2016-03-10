#ifndef A3_LIGHT_H
#define A3_LIGHT_H

#include <t3Math/core/t3Vector3.h>
#include <shapes/a3Shape.h>

class a3Ray;

class a3Light
{
public:
    a3Light();

    // a function for infinite area light
    virtual t3Vector3f Le(const a3Ray& ray);
};

#endif