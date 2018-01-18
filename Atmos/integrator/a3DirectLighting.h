#ifndef A3_DIRECTLIGHTING_H
#define A3_DIRECTLIGHTING_H

#include <integrator/a3Integrator.h>

class a3DirectLighting : public a3Integrator
{
public:
    a3DirectLighting();

    ~a3DirectLighting();

    virtual a3Spectrum Li(const a3Ray& ray, const a3Scene& scene) const;
};

#endif