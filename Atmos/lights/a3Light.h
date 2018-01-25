#ifndef A3_LIGHT_H
#define A3_LIGHT_H

#include <t3Math/core/t3Matri4x4.h>
#include <string>
#include <core/a3Spectrum.h>

class a3Ray;
class a3LightSamplingRecord;
class a3IntersectRecord;
class a3Light
{
public:    
    a3Light(std::string name);

    a3Light(const t3Matrix4x4& lightToWorld, std::string name);

    virtual ~a3Light();

    // return the radiant emittance for the attached shape's surface intersection
    virtual a3Spectrum eval(const a3IntersectRecord& its, const t3Vector3f &d) const;

    // return the radiant emittance from an environment emitter
    virtual a3Spectrum evalEnvironment(const a3Ray& ray) const;

    // given a reference point in the scene, sample an emitter position that contributes towards it.
    virtual a3Spectrum sampleDirect(a3LightSamplingRecord& dRec) const = 0;
    
    // is this an environment emitter
    virtual bool isEnvironment() const = 0;

    // is this a delta distribution emitter
    virtual bool isDeltaDistribution() const = 0;

    std::string name;

    t3Matrix4x4 lightToWorld, worldToLight;
};

#endif