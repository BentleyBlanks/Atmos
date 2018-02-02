#include <renderers/a3SamplerRenderer.h>

#include <core/a3Ray.h>
#include <core/image/a3Film.h>
#include <core/log/a3Log.h>
#include <core/a3Warp.h>
#include <core/random/a3Random.h>
#include <core/image/a3NormalMap.h>
#include <core/a3Utils.h>

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

void a3SamplerRenderer::render(const a3Scene* scene)
{
    if(!begin()) return;

    int imageWidth = camera->image->width;
    int imageHeight = camera->image->height;

#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < imageWidth; x++)
    {
        a3Log::info("Spp:%d    Rendering: %.2f%% \r", spp, (float) x / imageWidth * 100);

        for(int y = 0; y < imageHeight; y++)
        {
            a3Spectrum color;

            a3CameraSample sampleTentFilter, sample;

            for(int z = 0; z < spp; z++)
            {
                // super sampling
                sampler->getMoreSamples(x, y, &sample, &sampleTentFilter);

                a3Ray ray;

                // generate ray 
                camera->castRay(&sampleTentFilter, &ray);

                color += integrator->Li(ray, *scene) / spp;
            }

            colorList[x + y * imageWidth] = color;
            //colorList[x + y * imageWidth] = a3Spectrum(a3Random::randomFloat(), r.randomFloat(), r.randomFloat());
        }
    }
    a3Log::print("\n");

    t3Timer timer;
    timer.start();

    postEffect();

    timer.end();

    a3Log::info("Post Effec Cost: %f\n", timer.difference());

    // saving the result to the assigned disk path
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
        a3Log::error("Before a3SamplerRenderer::render() was called, the camera: %d is not allocaed\n", camera);
        return false;
    }

    if(!sampler)
    {
        a3Log::error("Before a3SamplerRenderer::render() was called, the sampler: %d is not allocaed\n", sampler);
        return false;
    }

    if(!integrator)
    {
        a3Log::error("Before a3SamplerRenderer::render() was called, the integrator: %d is not allocaed\n", integrator);
        return false;
    }

    return true;
}