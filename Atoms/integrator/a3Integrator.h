﻿#ifndef A3_INTERGRATOR_H
#define A3_INTERGRATOR_H

#include <core/a3Spectrum.h>
#include <core/a3Intersection.h>

class a3Ray;
class a3Scene;
class a3Sampler;
class a3BSDF;

class a3RadianceQueryRecord
{
public:
    a3RadianceQueryRecord();

    a3RadianceQueryRecord(const a3Scene *scene, a3Sampler *sampler);
    
    a3RadianceQueryRecord(const a3RadianceQueryRecord &record);

    // Initialize the query record for a recursive query
    inline void recursiveQuery(const a3RadianceQueryRecord &parent);

    // Current depth
    int depth;

    // Suface interaction data structure
    a3Intersection its;

    // Pointer to the associated scene
    const a3Scene* scene;

    // Sample generator
    a3Sampler* sampler;
};


class a3Integrator
{
public:
    virtual a3Spectrum li(const a3Ray& ray, const a3Scene& scene) const = 0;

    // 使用Multiple Importance Sampling完成直接光照采样
    // 这里wi, wo代表光线从摄像机出发的光路，内部计算中返回逆光路上的光照贡献
    a3Spectrum estimateDirect(const t3Vector3f& wi, const t3Vector3f& wo, bool& isDelta,
                              const a3Spectrum& bsdfValue, float bsdfPdf, const a3BSDF& bsdf, 
                              const a3Scene& scene, const a3Intersection& its) const;

//protected:
    a3Integrator() {}
    // Virtual destructor
    virtual ~a3Integrator() {}
};

class a3MonteCarloIntegrator : public a3Integrator
{
public:
    int maxDepth;

protected:
    // Virtual destructor
    a3MonteCarloIntegrator() {}

    virtual ~a3MonteCarloIntegrator() {}
};

#endif
 