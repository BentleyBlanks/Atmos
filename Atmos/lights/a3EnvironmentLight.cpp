#include <lights/a3EnvironmentLight.h>
#include <core/random/a3Random.h>
#include <core/a3Warp.h>
#include <core/a3Record.h>
#include <core/a3Ray.h>
#include <core/image/a3ImageDecoder.h>

a3EnvironmentLight::a3EnvironmentLight(const std::string & filePath, const t3Matrix4x4& objectToWorld)
    : objectToWorld(objectToWorld), a3Light("a3EnvironmentLight")
{
    image = new a3ImageDecoder(filePath);
}

a3EnvironmentLight::~a3EnvironmentLight()
{
    A3_SAFE_DELETE(image);
}

a3Spectrum a3EnvironmentLight::evalEnvironment(const a3Ray & ray) const
{
    t3Vector3f d = t3Matrix4x4::transform3x3(objectToWorld, ray.direction);

    // IS in the future
    float u = a3SphericalPhi(d) * T3MATH_INV_TWOPI;
    float v = a3SphericalTheta(d) * T3MATH_INV_PI;

    return image->lookup(u, v);
}

a3Spectrum a3EnvironmentLight::sampleDirect(a3LightSamplingRecord& dRec) const
{
    // IS in the future
    static a3Random r;

    dRec.pdf = 1 / (4 * PI);
    dRec.distance = A3_INFINITY;
    dRec.d = a3UniformSampleSphere(r.randomFloat(), r.randomFloat());

    // texture
    if(image)
    {
        float u = a3SphericalPhi(dRec.d) * T3MATH_INV_TWOPI;
        float v = a3SphericalTheta(dRec.d) * T3MATH_INV_PI;

        return image->lookup(u, v);
    }
    else
        return a3Spectrum::zero();
}

bool a3EnvironmentLight::isEnvironment() const
{
    return true;
}

bool a3EnvironmentLight::isDeltaDistribution() const
{
    return false;
}
