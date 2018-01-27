#ifndef A3_PATHTRACER_H
#define A3_PATHTRACER_H

#include <core/a3Spectrum.h>
#include <integrator/a3Integrator.h>

class a3Ray;
class a3Scene;

class a3PathTracer : public a3Integrator
{
public:
    a3PathTracer();

    virtual ~a3PathTracer();

    // Sample the incident radiance along a ray.
    virtual a3Spectrum Li(const a3Ray& ray, const a3Scene& scene) const;

    // Russian Rulette
    int maxDepth;
    int rrDepth;
};

#endif
