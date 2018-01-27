#ifndef A3_DIRECTLIGHTING_H
#define A3_DIRECTLIGHTING_H

#include <integrator/a3Integrator.h>

class a3DirectLighting : public a3Integrator
{
public:
    a3DirectLighting(int numBxdfSamples = 1, int numLightSamples = 1);

    ~a3DirectLighting();

    virtual a3Spectrum Li(const a3Ray& ray, const a3Scene& scene) const;

    // number of samples on light and bsdf
    int numLightSamples, numBxdfSamples;

    // MIS
    float fracBSDF, fracLight;
    float weightBSDF, weightLight;
};

#endif