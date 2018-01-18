#ifndef A3_LIGHT_H
#define A3_LIGHT_H

#include <t3Math/core/t3Matri4x4.h>
#include <string>
#include <core/a3Spectrum.h>

class a3Ray;
class a3LightSamplingRecord;

class a3Light
{
public:    
    a3Light(std::string name);

    a3Light(const t3Matrix4x4& lightToWorld, std::string name);

    virtual ~a3Light();

    virtual a3Spectrum evalEnvironment(const a3Ray& ray) const;

    virtual a3Spectrum sampleDirect(a3LightSamplingRecord& dRec) const = 0;

    virtual bool isEnvironment() const = 0;

    virtual bool isDeltaDistribution() const = 0;

    std::string name;

    t3Matrix4x4 lightToWorld, worldToLight;
};

#endif