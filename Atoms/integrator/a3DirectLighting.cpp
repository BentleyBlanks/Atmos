#include <integrator/a3DirectLighting.h>
#include <lights/a3Light.h>

//core
#include <core/a3Scene.h>
#include <core/a3Random.h>
#include <core/a3Ray.h>
#include <core/log/a3Log.h>
#include <core/a3Warp.h>

#include <bsdf/a3BSDF.h>
#include <samples/a3LightSample.h>

a3DirectLightingIntegrator::a3DirectLightingIntegrator() :maxDepth(1), bEnableRecursiveSampling(true)
{

}

a3DirectLightingIntegrator::a3DirectLightingIntegrator(int maxDepth, bool bEnableRecursiveSampling) : maxDepth(maxDepth), bEnableRecursiveSampling(bEnableRecursiveSampling)
{

}

// !first ray intersection
//      evironment lighting
//
// self-emmision
// 
// !bsdf.isDelta
//      [weight bsdf and light source to compute radiance]->1
//
// [optional]
// bsdf.isDelta
//      [recursive compute radiance]->2

// 一般不做直接光采样的积分器都采用bsdf sampling only形式
// 即从bsdf上发出的光线有否相交到有一定面积的光源
// 限制就是画面的质量依赖于光源的面积大小 无法处理point light/spot light
// 若只是做light sampling 那么对于Area Light就较难处理, 只能随机sample位置后计算weight
// 例如通过随机采样Infinite light而无法在合适的位置得到清晰的画面
// combine两者具体做法就是根据当前bsdf/light的pdf,heuristic()二者在不同环境下各自对radiance贡献的重要性程度
// 当光源为点光源形式,那么在bsdf sampling部分其weight就为0,而在light sampling部分计算其贡献
// 当bsdf为镜面/玻璃材质,在light sampling部分其weight为0，因为指定光路在指定方向上的贡献为0，而在bsdf sampling中计算其贡献
// 可以简单的将整个过程都视为Direct Lighting光照计算部分

// [1]对应于pbrt中的EstimateDirect()
// 
// light sampling / direct lighting
//      shadow ray
//      if !occulud
//          compute light contribution
//
// bsdf sampling
//      !light.isDelta
//          new ray
//          if intersect on light
//              compute light contribution

// 递归/增量计算delta形式的bsdf本质上与direct lighting过程无关
// 但可以更好的对比于indirect lighting的区别
// 仅有当bsdf为delta分布时才会继续向下增长光线路径
// 在当遇到非delta分布bsdf时Direct Lighting光照计算作为整条路径的radiance

// [2]对应于pbrt中的Specular/Transmission部分的递归
// while depth < max depth
//      bsdf.isDelta
//          find new ray direction
//
//          path through *= reflectivity / transmission
//
//          !scene.intersect
//              evironment lighting
//      !bsdf.isDelta
//          return path through * [1]
//
//      depth++
// end
a3Spectrum a3DirectLightingIntegrator::li(const a3Ray& ray, const a3Scene& scene) const
{
    /* ==================================================================== */
    /*                          First Ray Intersection                      */
    /* ==================================================================== */
    a3Spectrum Li, Le;
    a3Intersection its;

    // 第一次相交光线出射方向
    t3Vector3f wo;

    // 执行第一次Ray cast测试
    if(!scene.intersect(ray, &its))
    {
        // 添加可能有的无限远区域光照
        Li += scene.le(ray);
        return Li;
    }

    /* ==================================================================== */
    /*                             Estimate Direct                          */
    /* ==================================================================== */
    // 自发光统计贡献
    Le += its.shape->emission;

    // 获取相交shape上的bsdf
    a3BSDF* bsdf = its.shape->getBSDF();
    
    float bsdfPdf = 0.0f;

    // 初始化出射光线方向信息
    a3Spectrum bsdfValue = bsdf->sample(ray.direction, wo, &bsdfPdf, its);

    if(bsdfPdf == 0.0f)
        return Li;

    float cosTheta = t3Math::Abs(wo.dot(its.getNormal()));

    if(!bsdf->isDiracDistribution())
    {
        // --!暂未使用
        bool isDelta = false;
        // 根据多重重要性采样得到光照在逆光线方向上的贡献
        Li += estimateDirect(ray.direction, wo, isDelta, bsdfValue, bsdfPdf, *bsdf, scene, its);
    }
    else
    {
        /* ==================================================================== */
        /*                          Recursive sampling                          */
        /* ==================================================================== */
        // delta分布的bsdf光照计算需要在递归寻找到第一个非delta分布的bsdf为止才做直接光计算
        if(bEnableRecursiveSampling)
        {
            a3Spectrum recursiveLe, recursiveLi;
            a3Ray recursiveRay(its.p, wo);

            recursiveSampling(recursiveRay, scene, recursiveLe, recursiveLi);

            Le += recursiveLe;
            Li += recursiveLi;
        }
    }

    return Le + Li;
}

void a3DirectLightingIntegrator::recursiveSampling(const a3Ray& ray, const a3Scene& scene, 
                                                         a3Spectrum& Le, a3Spectrum& Li) const
{
    a3Ray recursiveRay(ray);
    a3Intersection its;
    // 下一递归相交光线出射方向
    t3Vector3f wo;

    // 路径上所有brdf * cosTheta / bsdfPdf累乘
    a3Spectrum throughput(1.0f);

    for(int depth = 0; depth < maxDepth; depth++)
    {
        /* ==================================================================== */
        /*                          First Ray Intersection                      */
        /* ==================================================================== */
        if(!scene.intersect(recursiveRay, &its))
        {
            Li += scene.le(recursiveRay);
            return;
        }

        /* ==================================================================== */
        /*                             Estimate Direct                          */
        /* ==================================================================== */
        // 自发光统计贡献
        Le += its.shape->emission;

        // 获取相交shape上的bsdf
        a3BSDF* bsdf = its.shape->getBSDF();

        float bsdfPdf = 0.0f;

        // 初始化出射光线方向信息
        a3Spectrum bsdfValue = bsdf->sample(recursiveRay.direction, wo, &bsdfPdf, its);

        if(bsdfPdf == 0.0f)
            return;

        if(!bsdf->isDiracDistribution())
        {
            // --!未启用
            bool isDelta = false;

            // 根据多重重要性采样得到光照贡献
            Li += estimateDirect(recursiveRay.direction, wo, isDelta, bsdfValue, bsdfPdf, *bsdf, scene, its);

            Li *= throughput;

            return;
        }
        
        float cosTheta = t3Math::Abs(wo.dot(its.getNormal()));

        // 因Delta Distribution的表面无需考虑cosTheta
        throughput *= bsdfValue * cosTheta / bsdfPdf;

        // 下一轮递增式光线跟踪
        recursiveRay.set(its.p, wo);
    }

    // 始终为delta分布的bsdf间弹射
}
