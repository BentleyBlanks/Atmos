#include <core/a3Record.h>
#include <lights/a3AreaLight.h>
#include <shapes/a3Shape.h>
#include <bsdf/a3BSDF.h>

// ------------------------------a3IntersectRecord------------------------------
bool a3IntersectRecord::isValid() const
{
    return t != FLT_MAX;
}

t3Vector3f a3IntersectRecord::getNormal() const
{
    if(shape)
        return shape->getNormal(p, u, v);
    else
        return t3Vector3f::zero();
}
a3BSDF * a3IntersectRecord::getBSDF() const
{
    return shape->getBSDF();
}