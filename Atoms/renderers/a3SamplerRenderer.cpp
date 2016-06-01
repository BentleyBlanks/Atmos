#include <renderers/a3SamplerRenderer.h>

#include <core/a3Ray.h>
#include <core/image/a3Film.h>
#include <core/log/a3Log.h>
#include <core/a3Warp.h>
#include <core/a3Random.h>
#include <core/image/a3NormalMap.h>

#include <sensors/a3Sensor.h>
#include <lights/a3Light.h>
#include <shapes/a3Shape.h>
#include <integrator/a3Integrator.h>

#include <samples/a3Sampler.h>
#include <samples/a3CameraSample.h>

#include <Common/t3Timer.h>

a3SamplerRenderer::a3SamplerRenderer()
: spp(128), sampler(NULL), camera(NULL), enableGammaCorrection(true), enableToneMapping(false), colorList(NULL)
{

}

a3SamplerRenderer::a3SamplerRenderer(int spp)
: spp(spp), sampler(NULL), camera(NULL), enableGammaCorrection(true), enableToneMapping(false), colorList(NULL)
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
    if(!begin()) return;

    int imageWidth = camera->image->width;
    int imageHeight = camera->image->height;

#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < imageWidth; x++)
    {
        a3Log::info("Spp:%d    Rendering: %8.2f \r", spp, (double) x / imageWidth * 100);

        for(int y = 0; y < imageHeight; y++)
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

                color += integrator->li(ray, *scene) / spp;
            }

            // 临时空间中setColor
            colorList[x + y * imageWidth] = color;
        }
    }
    a3Log::print("\n");

    t3Timer timer;
    timer.start();
    // 后期特效处理
    postEffect();
    timer.end();

    a3Log::info("Post Effec Cost: %f\n", timer.difference());

    // 保存真实渲染图像文件
    camera->image->write();

    end();
}

bool a3SamplerRenderer::begin()
{
    if(!check())
    {
        a3Log::warning("Rendering Start Failed\n");
        return false;
    }

    int imageWidth = camera->image->width;
    int imageHeight = camera->image->height;

    colorList = new a3Spectrum[imageWidth * imageHeight]();

    a3Log::success("Rendering Started\n");
    return true;
}

void a3SamplerRenderer::end()
{
    delete colorList;
    colorList = NULL;

    a3Log::success("Rendering Ended\n");
}

void a3SamplerRenderer::postEffect()
{
    int imageWidth = camera->image->width;
    int imageHeight = camera->image->height;

    a3Log::success("Post Effct Started\n");

    if(!enableToneMapping)
    {
        // clamp color
#pragma omp parallel for schedule(dynamic)
        for(int x = 0; x < imageWidth; x++)
        {
            a3Log::info("Clamp: %8.2f \r", (double) x / imageWidth * 100);

            for(int y = 0; y < imageHeight; y++)
            {
                a3Spectrum& color = colorList[x + y * imageWidth];

                color.x = t3Math::clamp(color.x, 0.0f, 1.0f);
                color.y = t3Math::clamp(color.y, 0.0f, 1.0f);
                color.z = t3Math::clamp(color.z, 0.0f, 1.0f);
            }
        }
        a3Log::print("\n");
    }

    // tone mapping
    a3Log::info("Tone Mapping: %s\n", enableToneMapping ? "enabled" : "disabled");

    if(enableToneMapping)
        a3ToneMapping(colorList, imageWidth, imageHeight);

#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < imageWidth; x++)
    {
        a3Log::info("Gamma Correction:%s    Imaging: %8.2f \r", enableGammaCorrection ? "enabled" : "disabled", (double) x / imageWidth * 100);

        for(int y = 0; y < imageHeight; y++)
        {
            a3Spectrum& color = colorList[x + y * imageWidth];

            // gamma correction
            if(enableGammaCorrection)
                a3GammaCorrection(color);

            camera->image->addColor(x, y, color);
        }
    }

    a3Log::print("\n");
    a3Log::success("Post Effct Ended\n");
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