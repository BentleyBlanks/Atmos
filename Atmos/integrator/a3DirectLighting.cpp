#include <integrator/a3DirectLighting.h>
#include <bsdf/a3BSDF.h>

// core
#include <core/a3Ray.h>
#include <core/a3Scene.h>
#include <core/a3Record.h>

// --!Learned from [Mitsuba](http://www.mitsuba-renderer.org/)
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

    // perform the first ray intersection
    if(!scene.intersect(ray, &its))
    {
        // add environment radiance if ray hit nothing
        L += scene.evalEnvironment(ray);
        return L;
    }

    // possibly include emitted radiance
    L += scene.Le(-ray.direction, its);

    // Estimate Direct Lighting
    a3LightSamplingRecord lRec(its.p, its.getNormal());

    const a3BSDF* bsdf = its.getBSDF();

    // ===========================================Light sampling ===========================================
    // Only use direct illumination sampling when the surface's BSDF is not delta distribution
    if(bsdf && !bsdf->isDeltaDistribution())
    {
        // direct illumination
        a3Spectrum value = scene.sampleDirect(lRec);

        if(value != a3Spectrum::zero())
        {
            // wi, wo always point away scattering event
            a3BSDFSamplingRecord bRec(its, -ray.direction, -lRec.d);

            a3Spectrum bsdfValue = bsdf->eval(bRec);

            // Evaluate BSDF(wi, wo) * cos(theta_o)
            if(bsdfValue != a3Spectrum::zero())
            {
                //--!MIS in the future
                L += value * bsdfValue;
            }
        }
    }

    return L;
}
