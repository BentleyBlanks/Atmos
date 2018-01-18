#ifndef A3_ENVIRONMENT_LIGHT_H
#define A3_ENVIRONMENT_LIGHT_H

#include <lights/a3Light.h>
#include <t3Math/core/t3Matri4x4.h>

class a3ImageDecoder;

class a3EnvironmentLight : public a3Light
{
public:
    // optional emmiter to world transform
    a3EnvironmentLight(const std::string& filePath, const t3Matrix4x4& objectToWorld = t3Matrix4x4::newIdentityMatrix());

    ~a3EnvironmentLight();

    virtual a3Spectrum evalEnvironment(const a3Ray& ray) const;

    virtual a3Spectrum sampleDirect(a3LightSamplingRecord& dRec) const;

    virtual bool isEnvironment() const;

    virtual bool isDeltaDistribution() const;

    // image based lighting
    a3ImageDecoder* image;

    // Specifies an emitter-to-world space rotation
    t3Matrix4x4 objectToWorld;
};

#endif 
