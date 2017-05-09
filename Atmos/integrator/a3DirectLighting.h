#ifndef A3_DIRECTLIGHTINGINTERGRATOR_H
#define A3_DIRECTLIGHTINGINTERGRATOR_H

#include <integrator/a3Integrator.h>

class a3DirectLightingIntegrator : public a3Integrator
{
public:
    a3DirectLightingIntegrator();
    a3DirectLightingIntegrator(int maxDepth, bool bEnableRecursiveSampling);
    virtual ~a3DirectLightingIntegrator();

    virtual a3Spectrum li(const a3Ray& ray, const a3Scene& scene) const;

    // 与li如出一辙的计算递归计算方式
    void recursiveSampling(const a3Ray& ray, const a3Scene& scene, 
                                 a3Spectrum& Le, a3Spectrum& Li) const;

    int maxDepth;

    bool bEnableRecursiveSampling;
};

#endif