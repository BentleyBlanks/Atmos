#ifndef A3_SCENE_H
#define A3_SCENE_H

#include <core/a3Settings.h>

class a3Intersection;
class a3Shape;
class a3Light;
class a3Ray;

class a3Scene
{
public:
	a3Scene();

    bool intersect(const a3Ray& ray, a3Intersection* intersection) const;

    bool intersect(const a3Ray& ray) const;

    bool addShape(a3Shape* shape);

    bool addLight(a3Light* light);

    // �������ж���͹�Դ
    std::vector<a3Shape*> objects;

    std::vector<a3Light*> lights;
};

#endif