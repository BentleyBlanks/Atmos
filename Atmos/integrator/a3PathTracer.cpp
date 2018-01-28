#include <integrator/a3PathTracer.h>
#include <bsdf/a3BSDF.h>

// core
#include <core/a3Ray.h>
#include <core/a3Scene.h>
#include <core/a3Record.h>
#include <core/a3Warp.h>
#include <shapes/a3Shape.h>
#include <core/random/a3Random.h>

a3PathTracer::a3PathTracer() :rrDepth(5), maxDepth(-1)
{
}

a3PathTracer::~a3PathTracer()
{
}

a3Spectrum a3PathTracer::Li(const a3Ray & r, const a3Scene & scene) const
{
    static a3Random random;

    a3Spectrum L;
    a3IntersectRecord its;
    a3Ray ray(r);
    int depth = 0;

    // perform the first ray intersection
    scene.intersect(ray, &its);

    a3Spectrum throughput(1.0f);
    float eta = 1.0f;

    while(depth <= maxDepth || maxDepth < 0)
    {
        if(!its.isValid())
        {
            // add environment radiance if ray hit nothing
            L += throughput * scene.evalEnvironment(ray);
            break;
        }

        if(its.isLight())
            // possibly include emitted radiance
            L += throughput * scene.Le(-ray.direction, its);

        const a3BSDF* bsdf = its.getBSDF();
        if(!bsdf)
            break;

        // The current path length is exceed the specifed maximum
        if(depth >= maxDepth && maxDepth > 0)
            break;
        
        // using multiple importance sampling to compute direct illumination
        // ===========================================Light Sampling===========================================
        // Estimate Direct Lighting
        a3LightSamplingRecord lRec(its.p, its.getNormal());
        if(!bsdf->isDeltaDistribution())
        {
            // direct illumination
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
                    float weight = a3MiWeight(lRec.pdf, bsdfPdf);
                    L += throughput * value * bsdfValue * weight;
                }
            }
        }

        // ===========================================BSDF Sampling===========================================
        lRec.set(its.p, its.getNormal());

        // Sample BSDF * cos(theta) and request the local density, wo
        a3BSDFSamplingRecord bRec(its, its.toLocal(-ray.direction));
        a3Spectrum bsdfValue = bsdf->sample(bRec);
        if(bsdfValue == a3Spectrum::zero())
            break;

        // convert shading coordinate to world
        t3Vector3f wo = its.toWorld(bRec.wo);

        // wo normal on the same side
        t3Vector3f n = its.getNormal();
        if(wo.dot(its.getNormal()) <= 0)
            break;

        // would be set by a area light or a environment light
        a3Spectrum value;

        bool hitLight = false;

        // Trace a ray in this direction
        ray = a3Ray(its.p, wo);
        // intersection test to find light
        if(scene.intersect(ray, &its))
        {
            // area light
            if(its.isLight())
            {
                lRec.light = its.shape->getLight();
                value = its.Le(-ray.direction);
                hitLight = true;
            }
        }
        else
        {
            lRec.light = scene.getEnvironmentLight();
            if(lRec.light)
            {
                // environment light(may not existed)
                value = scene.evalEnvironment(ray);
                hitLight = true;
            }
            else
                break;
        }

        // Keep track of the throughput and relative refractive index along the path
        throughput *= bsdfValue;
        eta *= bRec.eta;

        // If a light was hit, estimate the local illumination and weight using the power heuristic
        if(hitLight)
        {
            // Compute probility of generating that direction using light sampling
            float lightPdf = scene.pdfLightDirect(lRec);

            // Multiple importance sampling
            float weight = a3MiWeight(bRec.pdf, lightPdf);

            L += throughput * value * bsdfValue * weight;
        }

        // ===========================================Indirect illumination===========================================
        // Set the recursive query type. Stop if no surface was hit by the BSDF sample
        if(!its.isValid())
            break;

        // Russian roulette Stop with at least some probability to avoid getting stuck
        if(depth++ >= rrDepth)
        {
            float q = t3Math::Min(0.95f, a3RGB2Luminance(throughput));
            if(random.randomFloat() >= q)
                break;
            throughput /= q;
        }
    }

    return L;
}
