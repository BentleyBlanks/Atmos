#include <core/a3Scene.h>
#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/a3Intersection.h>
#include <lights/a3Light.h>
#include <core/log/a3Log.h>

a3Scene::a3Scene()
{

}

bool a3Scene::intersect(const a3Ray& ray, a3Intersection* intersection) const
{
    float minT = FLT_MAX;
    float t = FLT_MAX;

    for(int i = 0; i<objects.size(); i++)
    {
        if((t = objects[i]->intersect(ray)) && t < minT)
        {
            minT = t;

            // 序号代替ID
            intersection->shapeID = i;
        }
    }

    if(minT < FLT_MAX)
    {
        intersection->t = minT;
        return true;
    }
    else
    {
        intersection->t = 0;
        return false;
    }
}

bool a3Scene::intersect(const a3Ray& ray) const
{
    float minT = FLT_MAX;
    float t = FLT_MAX;

    for(int i = 0; i < objects.size(); i++)
    {
        if((t = objects[i]->intersect(ray)) && t < minT)
            minT = t;
    }

    return minT < FLT_MAX;
}

bool a3Scene::addShape(a3Shape* shape)
{
    if(!shape)
    {
        a3Log::error("空的形状对象指针%d\n", shape);
        return false;
    }

    objects.push_back(shape);
}

bool a3Scene::addLight(a3Light* light)
{
    if(!light)
    {
        a3Log::error("空的形状对象指针%d\n", light);
        return false;
    }

    lights.push_back(light);
}

