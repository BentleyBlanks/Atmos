#ifndef A3_SCENE_H
#define A3_SCENE_H

#include <core/a3Settings.h>
#include <core/a3Spectrum.h>

class a3IntersectRecord;
class a3LightSamplingRecord;
class a3Shape;
class a3Light;
class a3Ray;
class a3PrimitiveSet;

class a3Scene
{
public:
	a3Scene();
    ~a3Scene();

    // utility func
    bool addShape(a3Shape* shape);

    bool addLight(a3Light* light);

    bool check() const;

    // intersection with hit point detail
    bool intersect(const a3Ray& ray, a3IntersectRecord* intersection) const;

    // fast scene intersection test
    bool intersect(const a3Ray& ray) const;

    // Return the environment radiance for a ray that did not intersect any of the scene objects
    a3Spectrum evalEnvironment(const a3Ray& ray) const;

    // given a reference point in the scene, sample an emitter position that contributes towards it
    a3Spectrum sampleDirect(a3LightSamplingRecord& dRec) const;
    
    // returns radiance emitted into direction d
    a3Spectrum Le(const t3Vector3f& d, const a3IntersectRecord& intersection) const;

    // all lights in the scene
    std::vector<a3Light*> lights;

    // all shapes in the scene
    a3PrimitiveSet* primitiveSet;
};

#endif