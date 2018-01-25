#include <lights/a3Light.h>
#include <core/a3Scene.h>
#include <core/a3Utils.h>

a3Light::a3Light(const t3Matrix4x4& lightToWorld, std::string name)
    : lightToWorld(lightToWorld), worldToLight(lightToWorld.getInverse()), name(name)
{

}

a3Light::a3Light(std::string name) : name(name)
{
    lightToWorld.makeIdentityMatrix();
    worldToLight.makeIdentityMatrix();
}

a3Light::~a3Light()
{

}

a3Spectrum a3Light::eval(const a3IntersectRecord & its, const t3Vector3f & d) const
{
    return a3Spectrum::zero();
}

a3Spectrum a3Light::evalEnvironment(const a3Ray & ray) const
{
    return a3Spectrum::zero();
}

