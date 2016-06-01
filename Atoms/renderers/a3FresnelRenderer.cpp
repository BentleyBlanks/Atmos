#include <renderers/a3FresnelRenderer.h>
#include <core/log/a3Log.h>
#include <shapes/a3Shape.h>
#include <sensors/a3Sensor.h>

#include <core/a3Scene.h>
#include <core/log/a3Log.h>
#include <core/image/a3NormalMap.h>
#include <core/a3Ray.h>
#include <core/a3Intersection.h>
#include <t3Math/core/t3Math.h>

#include <samples/a3Sampler.h>
#include <samples/a3CameraSample.h>

float a3FrDiel(float cosi, float cost, const float &etai, const float &etat)
{
    float Rparl = ((etat * cosi) - (etai * cost)) /
        ((etat * cosi) + (etai * cost));
    float Rperp = ((etai * cosi) - (etat * cost)) /
        ((etai * cosi) + (etat * cost));
    return (Rparl*Rparl + Rperp*Rperp) / 2.f;
}

void a3FresnelRenderer::render(const a3Scene* scene)
{
    // 渲染法线贴图用于检测法线
#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < camera->image->width; x++)
    {
        a3Log::info("Fresnel Map    Rendering: %8.2f \r", (double) x / camera->image->width * 100);

        for(int y = 0; y < camera->image->height; y++)
        {
            // 当前采样位置
            a3CameraSample sample;
            a3Ray ray;

            // 获取下一个采样位置
            sampler->getMoreSamples(x, y, &sample);

            camera->castRay(&sample, &ray);

            float fresnel = getFresnel(scene, &ray, &sample);

            image->addSample(&sample, t3Vector3f(fresnel * 255.0f));
        }
    }

    a3Log::print("\n");
    // 保存法线图像文件
    image->write();
}

bool a3FresnelRenderer::check()
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

    if(!image)
    {
        a3Log::error("a3SamplerRenderer::render()前image: %d尚未分配指定\n", image);
        return false;
    }

    return true;
}

float a3FresnelRenderer::getFresnel(const a3Scene* scene, a3Ray* ray, const a3CameraSample* sample)
{
    a3Intersection intersection;

    if(!scene->intersect(*ray, &intersection))
        // zero normal
        return 0.0f;

    a3Shape* obj = intersection.shape;

    t3Vector3f normal = intersection.getNormal();

    float n = obj->refractiveIndex;

    //float R0 = (1.0f - n) / (1.0 + n);

    //R0 = R0 * R0;

    float costheta1 = normal.dot(-ray->direction);

    //float probablity = R0 + (1 - R0) * t3Math::pow((1 - t3Math::Abs(costheta1)), 5);

    float sint = 1.0f / n * sqrtf(1.f - costheta1 * costheta1);

    float cost = sqrtf(1.f - sint * sint);

    float probablity = a3FrDiel(costheta1, cost, 1.0f, n);

    return probablity;
}

