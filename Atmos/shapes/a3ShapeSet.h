#ifndef A3_SHAPESET_H
#define A3_SHAPESET_H

#include <t3Math/core/t3Vector3.h>
#include <vector>

class a3Shape;
class a3LightSample;

class a3ShapeSet
{
public:
    a3ShapeSet();

    ~a3ShapeSet();

    // compute shape list's sum area
    float area() const;

    // get normal and sample point
    t3Vector3f sample(const a3LightSample& sample, t3Vector3f* normal) const;

    float pdf(const t3Vector3f& p, const t3Vector3f& wi) const;

    bool addShape(a3Shape* shape);
    
    float sumArea;

    std::vector<a3Shape*> shapes;

    std::vector<float> areas;
};

#endif