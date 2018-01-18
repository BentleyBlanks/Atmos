#include <accelerators/a3Exhaustive.h>
#include <core/a3Record.h>
#include <shapes/a3Sphere.h>
#include <core/a3Ray.h>

bool a3Exhaustive::intersect(const a3Ray& ray, a3IntersectRecord* its) const
{
    float minT = FLT_MAX;
    float t = FLT_MAX;
    float u = 0, v = 0;
    float vtu = 0, vtv = 0;

    for(int i = 0; i < primitives.size(); i++)
    {
        //if((primitives[i]->intersect(ray, &t, &u, &v, &vtu, &vtv)) && t > A3_TOLERANCE_FLOAT && t < minT)
        if((primitives[i]->intersect(ray, &t, &u, &v)) && t > A3_EPSILON_FLOAT && t < minT)
        {
            minT = t;

            its->shape = primitives[i];
        }
    }

    if(minT < FLT_MAX && minT > A3_EPSILON_FLOAT)
    {
        its->t = minT;
        // hit point
        its->p = (ray) (minT);

        // triangle only
        its->u = u;
        its->v = v;

        // texture coordinate
        //its->vtu = vtu;
        //its->vtv = vtv;

        return true;
    }
    else
    {
        // ÓÃÓÚvalidÅÐ¶Ï
        its->t = FLT_MAX;

        return false;
    }
}

bool a3Exhaustive::intersect(const a3Ray& ray) const
{
    float minT = FLT_MAX;
    float t = FLT_MAX;
    float u = 0, v = 0;
    float vtu = 0, vtv = 0;

    for(int i = 0; i<primitives.size(); i++)
    {
        //if((primitives[i]->intersect(ray, &t, &u, &v, &vtu, &vtv)) && t > A3_TOLERANCE_FLOAT && t < minT)
        if((primitives[i]->intersect(ray, &t, &u, &v)) && t > A3_EPSILON_FLOAT && t < minT)
            minT = t;
    }

    if(minT < ray.maxT && minT > ray.minT)
        return true;
    else
        return false;
}

