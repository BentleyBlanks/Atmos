#include <lights/a3AreaLight.h>
#include <core/log/a3Log.h>
#include <core/a3Utils.h>
#include <core/a3Record.h>
#include <shapes/a3Shape.h>

a3AreaLight::a3AreaLight(a3Shape* shape, a3Spectrum radiance, const t3Matrix4x4 & lightToWorld)
    : radiance(radiance), a3Light(lightToWorld, "a3AreaLight")
{
    // attach a shape to a light
    this->shape = shape;
    shape->setLight(this);
}

a3AreaLight::~a3AreaLight()
{
}

a3Spectrum a3AreaLight::eval(const a3IntersectRecord & its, const t3Vector3f & d) const
{
    if(its.getNormal().dot(d) > 0.0f)
        return radiance;
    else
        return a3Spectrum::zero();
}

a3Spectrum a3AreaLight::sampleDirect(a3LightSamplingRecord & lRec) const
{
    if(!shape)
    {
        a3NullPtrWarning("shape");
        return a3Spectrum::zero();
    }
    
    a3ShapeSamplingRecord sRec;
    shape->sample(sRec);
    
    // filling the sampling info
    lRec.p = sRec.p;
    lRec.normal = sRec.normal;

    lRec.d = lRec.hitPoint - lRec.p;
    lRec.distance = lRec.d.length();
    lRec.d.normalize();

    // light to the hitPoint respect to normal on light
    float cosTheta = t3Math::Abs(lRec.normal.dot(lRec.d));
    // pdf area to solid angle
    lRec.pdf = cosTheta != 0 ? sRec.pdf * lRec.distance * lRec.distance / cosTheta : 0.0f;

    // light and hitPoint are oriented correctly with respect to each other
    if(lRec.normal.dot(lRec.d) >= 0.0f && 
       lRec.hitNormal.dot(lRec.d) <= 0.0f &&
       lRec.pdf != 0.0f)
        return radiance / lRec.pdf;
    else
        return a3Spectrum::zero();
}

bool a3AreaLight::isEnvironment() const
{
    return false;
}

bool a3AreaLight::isDeltaDistribution() const
{
    return false;
}

void a3AreaLight::setShape(a3Shape * shape)
{
    if(shape)
        this->shape = shape;
    else
        a3NullPtrWarning("shape");
}

const a3Shape * a3AreaLight::getShape()
{
    return shape;
}
