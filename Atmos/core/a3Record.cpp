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

t3Vector3f a3IntersectRecord::toWorld(const t3Vector3f & localCoord)
{
    return t3Matrix4x4::transform3x3(localToWorld, localCoord).getNormalized();
}

t3Vector3f a3IntersectRecord::toLocal(const t3Vector3f & worldCoord)
{
    return t3Matrix4x4::transform3x3(worldToLocal, worldCoord).getNormalized();
}

void a3LightSamplingRecord::set(const t3Vector3f & hitPoint, const t3Vector3f & hitNormal)
{
    light = NULL;
    this->hitPoint = hitPoint;
    this->hitNormal = hitNormal;
    pdf = 0.0f;
    distance = 0.0f;
}
