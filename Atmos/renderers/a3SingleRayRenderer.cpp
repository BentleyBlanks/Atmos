#include <renderers/a3SingleRayRenderer.h>
#include <integrator/a3Integrator.h>
#include <sensors/a3Sensor.h>

#include <core/image/a3Film.h>
#include <core/a3Warp.h>
#include <core/log/a3Log.h>
#include <core/a3Ray.h>

#include <samples/a3Sampler.h>
#include <samples/a3CameraSample.h>

a3SingleRayRenderer::a3SingleRayRenderer() :singleRayX(0), singleRayY(0)
{

}

a3SingleRayRenderer::a3SingleRayRenderer(int singleRayX, int singleRayY) : singleRayX(singleRayX), singleRayY(singleRayY)
{

}

a3SingleRayRenderer::~a3SingleRayRenderer()
{

}

void a3SingleRayRenderer::render(const a3Scene* scene)
{
    if(!check()) return;

    a3CameraSample sample;

    sampler->getMoreSamples(singleRayX, singleRayY, &sample, NULL);

    // memory allocating
    a3Ray ray;

    // generate camera rays
    camera->castRay(&sample, &ray);

    t3Vector3f color = integrator->Li(ray, *scene);

    color.x = t3Math::clamp(color.x, 0.0f, 255.0f);
    color.y = t3Math::clamp(color.y, 0.0f, 255.0f);
    color.z = t3Math::clamp(color.z, 0.0f, 255.0f);

    if(enableGammaCorrection)
        a3GammaCorrection(color);

    //color.print("SingleRay Color");

    a3Log::info("SingleRay Pos: %d, %d\n", singleRayX, singleRayY);

    a3Log::info("SingleRay Color: %f, %f, %f\n", color.x, color.y, color.z);
}

bool a3SingleRayRenderer::check()
{
    if(!camera)
    {
        a3Log::error("Before a3SamplerRenderer::render() was called, camera: %d is not allocated\n", camera);
        return false;
    }

    if(!sampler)
    {
        a3Log::error("Before a3SamplerRenderer::render() was called, sampler: %d is not allocated\n", sampler);
        return false;
    }

    if(!integrator)
    {
        a3Log::error("Before a3SamplerRenderer::render() was called, integrator: %d is not allocated\n", integrator);
        return false;
    }

    if(singleRayX < 0 || singleRayY < 0)
    {
        a3Log::error("Error coordinates:(%d %d) when a3SamplerRenderer::render() was called", singleRayX, singleRayY);

        return false;
    }

    return true;
}
