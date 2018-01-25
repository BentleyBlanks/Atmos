#ifndef A3_ENVIRONMENT_LIGHT_H
#define A3_ENVIRONMENT_LIGHT_H

#include <lights/a3Light.h>
#include <t3Math/core/t3Matri4x4.h>

class a3ImageDecoder;

class a3EnvironmentLight : public a3Light
{
public:
    // optional emmiter to world transform
    a3EnvironmentLight(const std::string& filePath, const t3Matrix4x4& lightToWorld = t3Matrix4x4::newIdentityMatrix());

    ~a3EnvironmentLight();

    // return the radiant emittance from an environment emitter
    virtual a3Spectrum evalEnvironment(const a3Ray& ray) const;

    // given a reference point in the scene, sample an emitter position that contributes towards it.
    virtual a3Spectrum sampleDirect(a3LightSamplingRecord& dRec) const;

    // is this an environment emitter
    virtual bool isEnvironment() const;

    // is this a delta distribution emitter
    virtual bool isDeltaDistribution() const;

    // image based lighting
    a3ImageDecoder* image;
};

#endif 
