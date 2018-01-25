#ifndef A3_AREA_LIGHT_H
#define A3_AREA_LIGHT_H

#include <lights/a3Light.h>
#include <t3Math/core/t3Matri4x4.h>
#include <core/a3Spectrum.h>

class a3ImageDecoder;
class a3Shape;
class a3AreaLight : public a3Light
{
public:
    // optional emmiter to world transform
    a3AreaLight(a3Shape* shape, a3Spectrum radiance, const t3Matrix4x4& lightToWorld = t3Matrix4x4::newIdentityMatrix());

    ~a3AreaLight();

    // return the radiant emittance for the attached shape's surface intersection
    virtual a3Spectrum eval(const a3IntersectRecord& its, const t3Vector3f &d) const;

    // given a reference point in the scene, sample an emitter position that contributes towards it.
    virtual a3Spectrum sampleDirect(a3LightSamplingRecord& dRec) const;

    // is this an environment emitter
    virtual bool isEnvironment() const;

    // is this a delta distribution emitter
    virtual bool isDeltaDistribution() const;

    // attach to a shape ptr
    void setShape(a3Shape* shape);

    const a3Shape* getShape();

    a3Shape* shape;

    a3Spectrum radiance;
};

#endif 
