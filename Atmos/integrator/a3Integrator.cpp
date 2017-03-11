#include <integrator/a3Integrator.h>
#include <bsdf/a3BSDF.h>

// core
#include <core/a3Ray.h>
#include <core/a3Warp.h>
#include <core/a3Scene.h>

#include <shapes/a3Shape.h>
#include <samples/a3LightSample.h>
#include <lights/a3Light.h>
#include <lights/a3AreaLight.h>

a3RadianceQueryRecord::a3RadianceQueryRecord()
    : scene(NULL), sampler(NULL), depth(0)
{

}

a3RadianceQueryRecord::a3RadianceQueryRecord(const a3Scene *scene, a3Sampler *sampler)
    : scene(scene), sampler(sampler), depth(0)
{

}

a3RadianceQueryRecord::a3RadianceQueryRecord(const a3RadianceQueryRecord &record)
    : scene(record.scene), sampler(record.sampler), depth(record.depth)
{

}

void a3RadianceQueryRecord::recursiveQuery(const a3RadianceQueryRecord &parent)
{
    scene = parent.scene;
    depth = parent.depth + 1;
    sampler = parent.sampler;
}

a3Spectrum a3Integrator::estimateDirect(const t3Vector3f& wi, const t3Vector3f& wo, bool& isDirac,
                                        const a3Spectrum& bsdfValue, float bsdfPdf, const a3BSDF& bsdf,
                                        const a3Scene& scene, const a3Intersection& its) const
{
    a3Spectrum Li;
    // --!Atmos暂时采用对所有光源进行随机采样抽取一个光源进行计算的方法
    a3Random random;
    a3Light* light = NULL;
    int length = scene.lights.size();

    isDirac = bsdf.isDiracDistribution();

    if(length > 0)
    {
        // --!未来应更改为[0, n)的整数随机序列
        int lightNum = (int) (random.randomDouble()) * length;

        // 随机选取光源
        light = scene.lights[lightNum];
    }

    float lightPdf = 0.0f;

    if(!light)
        return a3Spectrum::zero();

    /* ==================================================================== */
    /*                               Light Sampling                         */
    /* ==================================================================== */
    // 非镜面材质
    if(!bsdf.isDiracDistribution())
    {
        // 直接光照采样计算 + shadowRay test
        a3VisibilityTester vis;

        t3Vector3f incidentDirection;
        a3Spectrum lightValue = light->sampleL(incidentDirection, its.p, &lightPdf, a3LightSample(), vis);

        if(lightValue != a3Spectrum::zero())
        {
            a3Spectrum bsdfValue = bsdf.eval(-wi, -incidentDirection, its);

            // 光照方向的cos权重
            float cosTheta = t3Math::Abs(incidentDirection.dot(its.getNormal()));

            // 光源与材质之间没有遮挡物
            if(bsdfValue != a3Spectrum::zero() && !vis.occluded(&scene))
            {
                if(light->isDiracDistribution())
                    // Evaluate BSDF * cos(theta)
                    // 对满足dirac分布的光源进行直接光照
                    Li += lightValue * bsdfValue * cosTheta / lightPdf;
                else
                {
                    // 多重重要性采样，目前pdf部分其实并未实现
                    float bsdfPdf = bsdf.pdf(wi, wo, its);
                    // --!若这里BSDF的pdf较小，如高光模型，那么weight就会非常接近于0
                    // --!这种情况下，在光源上采样得到的直接光照辐射度
                    float weight = a3PowerHeuristic(1, lightPdf, 1, bsdfPdf);
                    Li += lightValue * bsdfValue * cosTheta * weight / lightPdf;
                }
            }
        }
    }

    /* ==================================================================== */
    /*                            BSDF sampling                             */
    /* ==================================================================== */
    // 只有当光源不为delta分布时才采用BSDF Sampling, 即出射光线有一定概率击中光源
    if(!light->isDiracDistribution())
    {
        if(bsdfValue != a3Spectrum::zero())
        {
            a3Ray bsdfRay(its.p, wo);
            a3Spectrum lightValue;
            a3Intersection lightIts;

            // 新跟踪一出射光线
            if(scene.intersect(bsdfRay, &lightIts))
            {
                // 是否为当前的面积光光照模型
                if(lightIts.shape->areaLight == light)
                    lightValue += lightIts.getEmission(wo);
            }
            else
            {
                // 无限远区域光源
                for(auto l : scene.lights)
                {
                    lightPdf = l->pdf(its.p, wo);
                    lightValue += l->le(bsdfRay);
                }
            }

            float cosTheta = t3Math::Abs(wo.dot(its.getNormal()));

            if(lightValue != a3Spectrum::zero())
            {
                // dirac分布的bsdf需要在外界显式计算
                if(!bsdf.isDiracDistribution())
                {
                    // --!若这里Light的pdf较小，如无限远区域光，那么weight就会非常接近于0
                    // --!这种情况下，在指定方向上计算得到的辐射度能够大幅度保留
                    float weight = a3PowerHeuristic(1, bsdfPdf, 1, lightPdf);
                    Li += lightValue * bsdfValue * cosTheta * weight / bsdfPdf;
                }
            }
        }
    }

    return length * Li;
}

