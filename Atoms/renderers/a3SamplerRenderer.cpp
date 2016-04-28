#include <renderers/a3SamplerRenderer.h>

#include <core/a3Ray.h>
#include <core/image/a3Film.h>
#include <core/log/a3Log.h>
#include <core/a3Warp.h>
#include <core/a3Random.h>
#include <core/image/a3NormalMap.h>
#include <core/a3Spectrum.h>

#include <cameras/a3PerspectiveSensor.h>
#include <lights/a3Light.h>
#include <shapes/a3Shape.h>
#include <integrator/a3Integrator.h>

#include <samples/a3Sampler.h>
#include <samples/a3CameraSample.h>

//#define A3_RENDERING_NORMALMAP
#define A3_RENDERING_REALISTICIMAGE
//#define A3_RENDERING_SINGLERAY

a3SamplerRenderer::a3SamplerRenderer()
: spp(128), sampler(NULL), camera(NULL), enableGammaCorrection(true)
{

}

a3SamplerRenderer::a3SamplerRenderer(int spp)
: spp(spp), sampler(NULL), camera(NULL), enableGammaCorrection(true)
{

}

a3SamplerRenderer::~a3SamplerRenderer()
{

}
// pbrt划分渲染队列独立任务的渲染管线
// |Task.run()| |Task.run()| ... |Task.run()|
// ------------------------------------------
// Task.run()
//      sample = RandomSampler.getSubSampler()
//      ray = Camera.generateRayDiffererntial(sample)
//      intersection = Scene.Intersect(ray)
//      spectrum = Intergrator.Li(intersection)
//      File.AddSample(spectrum, sample)
//      Reporter.Update()
void a3SamplerRenderer::render(const a3Scene* scene)
{
    if(!check()) return;

#ifdef A3_RENDERING_NORMALMAP
    // 渲染法线贴图用于检测法线
#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < camera->image->width; x++)
    {
        a3Log::info("Normal Map    Rendering: %8.2f \r", (double) x / camera->image->width * 100);

        for(int y = 0; y < camera->image->height; y++)
        {
            // 当前采样位置
            a3CameraSample sample;
            a3Ray ray;

            // 获取下一个采样位置
            sampler->getSample(x, y, &sample);

            camera->castRay(&sample, &ray);

            t3Vector3f n = getNormal(scene, &ray, &sample);

            camera->normalMap->addSample(&sample, n);
        }
    }

    a3Log::print("\n");
    // 保存法线图像文件
    camera->normalMap->write();
#endif

#ifdef A3_RENDERING_REALISTICIMAGE
#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < camera->image->width; x++)
    {
        a3Log::info("Spp:%d    Rendering: %8.2f \r", spp, (double) x / camera->image->width * 100);

        for(int y = 0; y < camera->image->height; y++)
        {
            a3Spectrum color;

            // 当前采样位置
            a3CameraSample sampleTentFilter, sample;

            for(int z = 0; z < spp; z++)
            {
                // 获取下一个采样位置
                sampler->getMoreSamples(x, y, &sample, &sampleTentFilter);

                // memory allocating
                a3Ray ray;

                // 生成光线
                camera->castRay(&sampleTentFilter, &ray);

                //a3RadianceQueryRecord record(scene, sampler);

                color += integrator->li(ray, *scene) / spp;
            }

            color.x = t3Math::clamp(color.x, 0.0f, 255.0f);
            color.y = t3Math::clamp(color.y, 0.0f, 255.0f);
            color.z = t3Math::clamp(color.z, 0.0f, 255.0f);

            //t3Vector3f toneColor = color;
            //t3Vector3f color = a3Tonemap(color / 255.0f);

            if(enableGammaCorrection)
                a3GammaCorrection(color);

            //if((color.x > 254 || color.y > 254 || color.z > 254) && y < 100)
            //{
            //    a3Log::error("Fuck!!!!! %d, %d\n", x, y);
            //}

            camera->image->addSample(&sample, color);
        }
    }
    a3Log::print("\n");

    // 保存真实渲染图像文件
    camera->image->write();
#endif

#ifdef A3_RENDERING_SINGLERAY
    int singleRayX = 352, singleRayY = 335;

    a3CameraSample sample;

    sampler->getMoreSamples(singleRayX, singleRayY, &sample, NULL);

    // memory allocating
    a3Ray ray;

    // 生成光线
    camera->castRay(&sample, &ray);

    t3Vector3f color = integrator->li(ray, *scene);

    color.x = t3Math::clamp(color.x, 0.0f, 255.0f);
    color.y = t3Math::clamp(color.y, 0.0f, 255.0f);
    color.z = t3Math::clamp(color.z, 0.0f, 255.0f);

    if(enableGammaCorrection)
        a3GammaCorrection(color);

    //color.print("SingleRay Color");

    a3Log::info("SingleRay Pos: %d, %d\n", singleRayX, singleRayY);

    a3Log::info("SingleRay Color: %f, %f, %f\n", color.x, color.y, color.z);
#endif
}

bool a3SamplerRenderer::check()
{
    if(!camera)
    {
        a3Log::error("a3SamplerRenderer::render()前camera: %d尚未分配指定\n", camera);
        return false;
    }

    if(!sampler)
    {
        a3Log::error("a3SamplerRenderer::render()前sampler: %d尚未分配指定\n", sampler);
        return false;
    }

    if(!integrator)
    {
        a3Log::error("a3SamplerRenderer::render()前integrator: %d尚未分配指定\n", integrator);
        return false;
    }

    return true;
}
