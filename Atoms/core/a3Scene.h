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

    // 场景所有对象和光源(临时球型光源也是Shape)
    std::vector<a3Shape*> objects;
};

#endif