#include <integrator/a3DirectLighting.h>

#include <bsdf/a3BSDF.h>

// core
#include <core/a3Ray.h>
#include <core/a3Scene.h>
#include <core/a3Record.h>

a3DirectLighting::a3DirectLighting()
{
}

a3DirectLighting::~a3DirectLighting()
{
}

a3Spectrum a3DirectLighting::Li(const a3Ray & ray, const a3Scene & scene) const
{
    a3Spectrum L;
    a3IntersectRecord its;

    if(!scene.intersect(ray, &its))
    {
        // add environment radiance if ray hit nothing
        L += scene.evalEnvironment(ray);
        return L;
    }

    a3LightSamplingRecord dRec(its.p, its.getNormal());

    // Emitter sampling 
    a3Spectrum value = scene.sampleDirect(dRec);

    a3BSDFSamplingRecord bRec(its, -ray.direction, dRec.d);

    float cosTheta = bRec.wo.dot(dRec.normal);
    if(cosTheta < 0)
        return L;

    // Ld * bsdf * cos\theta
    L += value * its.getBSDF()->pdf(bRec) * cosTheta;

    return L;
}
