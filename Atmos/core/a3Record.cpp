 #include <core/a3Record.h>
#include <lights/a3AreaLight.h>
#include <shapes/a3Shape.h>
#include <bsdf/a3BSDF.h>

// ------------------------------a3IntersectRecord------------------------------
bool a3IntersectRecord::isValid() const
{
    return t != FLT_MAX;
}

bool a3IntersectRecord::isLight() const
{
    return shape->isLight();
}

t3Vector3f a3IntersectRecord::getNormal() const
{
    if(shape)
        return shape->getNormal(p, u, v);
    else
        return t3Vector3f::zero();
}

const a3BSDF * a3IntersectRecord::getBSDF() const
{
    return shape->getBSDF();
}

a3Spectrum a3IntersectRecord::Le(const t3Vector3f & d) const
{
    a3Light* light = shape->getLight();
    return light ? light->eval(*this, d) : a3Spectrum::zero();
}
