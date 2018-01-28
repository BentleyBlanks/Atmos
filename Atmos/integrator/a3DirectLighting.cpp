#include <integrator/a3DirectLighting.h>
#include <bsdf/a3BSDF.h>

// core
#include <core/log/a3Log.h>
#include <core/a3Ray.h>
#include <core/a3Scene.h>
#include <core/a3Record.h>
#include <core/a3Warp.h>
#include <shapes/a3Shape.h>

// --!Learned from [Mitsuba](http://www.mitsuba-renderer.org/)
a3DirectLighting::a3DirectLighting(int numBxdfSamples, int numLightSamples)
    : numBxdfSamples(numBxdfSamples), numLightSamples(numLightSamples)
{
    int sum = numBxdfSamples + numLightSamples;

    weightBSDF = 1.0f / numBxdfSamples;
    weightLight = 1.0f / numLightSamples;
    fracBSDF = numBxdfSamples / (float) sum;
    fracLight = numLightSamples / (float) sum;
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

    if(its.isLight())
        // possibly include emitted radiance
        L += scene.Le(-ray.direction, its);

    // Estimate Direct Lighting
    a3LightSamplingRecord lRec(its.p, its.getNormal());

    const a3BSDF* bsdf = its.getBSDF();
    if(!bsdf) return L;

    // ===========================================Light Sampling ===========================================
    // Only use direct illumination sampling when the surface's BSDF is not delta distribution
    if(!bsdf->isDeltaDistribution())
    {
        for(int i = 0; i < numLightSamples; i++)
        {        
            // get (L / lightPdf) in direct illumination
            a3Spectrum value = scene.sampleDirect(lRec);

            if(value != a3Spectrum::zero())
            {
                // wi, wo always point away scattering event
                a3BSDFSamplingRecord bRec(its, its.toLocal(-ray.direction), its.toLocal(-lRec.d));

                a3Spectrum bsdfValue = bsdf->eval(bRec);

                // Evaluate BSDF(wi, wo) * cos(theta_o)
                if(bsdfValue != a3Spectrum::zero())
                {
                    // Calculate probility of sampling that direction using BSDF sampling
                    float bsdfPdf = bsdf->pdf(bRec);
                    
                    // Multiple importance sampling
                    float weight = a3MiWeight(lRec.pdf * fracLight, bsdfPdf * fracBSDF) * weightLight;

                    L += value * bsdfValue * weight;
                }
            }
        }
    }

    
    //int sum = 0;
    // ===========================================BSDF Sampling===========================================
    for(int i = 0; i < numBxdfSamples; i++)
    {
        a3LightSamplingRecord lRec(its.p, its.getNormal());

        // Sample BSDF * cos(theta) and request the local density, wo
        a3BSDFSamplingRecord bRec(its, its.toLocal(-ray.direction));
        a3Spectrum bsdfValue = bsdf->sample(bRec);
        if(bsdfValue == a3Spectrum::zero())
            continue;

        // convert shading coordinate to world
        t3Vector3f wo = its.toWorld(bRec.wo);

        // wo normal on the same side
        t3Vector3f n = its.getNormal();
        if(wo.dot(its.getNormal()) <= 0)
            continue;

        // trace a ray in wo
        a3Ray bsdfRay(its.p, wo);

        // would be set by a area light or a environment light
        a3Spectrum value;

        a3IntersectRecord bsdfIts;
        // intersection test to find light
        if(scene.intersect(bsdfRay, &bsdfIts))
        {
            // area light
            if(!bsdfIts.isLight())
                continue;

            //sum++;
            lRec.light = bsdfIts.getLight();
            value = bsdfIts.Le(-bsdfRay.direction);
        }
        else
        {
            lRec.light = scene.getEnvironmentLight();
            // environment light(may not existed)
            value = scene.evalEnvironment(ray);
        }

        if(value == a3Spectrum::zero())
            continue;

        // Compute probility of generating that direction using light sampling
        float lightPdf = scene.pdfLightDirect(lRec);

        // Multiple importance sampling
        float weight = a3MiWeight(bRec.pdf * fracBSDF, lightPdf * fracLight) * weightBSDF;

        L += value * bsdfValue * weight;
    }

    //a3Log::warning("Sum:%d\n", sum);
    return L;
}
