#ifndef A3_SCENE_H
#define A3_SCENE_H

#include <core/a3Settings.h>
#include <core/a3Spectrum.h>

class a3Intersection;
class a3Shape;
class a3Light;
class a3Ray;
class a3PrimitiveSet;

class a3Scene
{
public:
	a3Scene();
    ~a3Scene();

    a3Spectrum le(const a3Ray& ray) const;

    bool intersect(const a3Ray& ray, a3Intersection* intersection) const;

    bool intersect(const a3Ray& ray) const;

    bool addShape(a3Shape* shape);

    bool addLight(a3Light* light);

    bool isValid() const;

    bool check() const;

    // 场景所有光源
    std::vector<a3Light*> lights;

    // 图元集合
    a3PrimitiveSet* primitiveSet;
};

#endif