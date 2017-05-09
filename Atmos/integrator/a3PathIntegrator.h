#ifndef A3_PATH_H
#define A3_PATH_H

#include <integrator/a3Integrator.h>

class a3PathIntegrator : public a3MonteCarloIntegrator
{
public:
    a3PathIntegrator();
    a3PathIntegrator(int russianRouletteDepth, int maxDepth);
    virtual ~a3PathIntegrator();

    a3Spectrum li(const a3Ray& ray, const a3Scene& scene) const;

    // Russian Roulette Start Depth
    int russianRouletteDepth;
};

#endif