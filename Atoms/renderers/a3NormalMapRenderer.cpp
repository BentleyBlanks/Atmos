#include <renderers/a3NormalMapRenderer.h>
#include <shapes/a3Shape.h>
#include <sensors/a3Sensor.h>

#include <core/a3Scene.h>
#include <core/log/a3Log.h>
#include <core/image/a3NormalMap.h>
#include <core/a3Ray.h>
#include <core/a3Intersection.h>

#include <samples/a3Sampler.h>
#include <samples/a3CameraSample.h>

#include <core/image/a3NormalMap.h>

void a3NormalMapRenderer::render(const a3Scene* scene)
{
    if(!check()) return;

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
            sampler->getMoreSamples(x, y, &sample);

            camera->castRay(&sample, &ray);

            t3Vector3f n = getNormal(scene, &ray, &sample);

            normalMap->addSample(&sample, n);
        }
    }

    a3Log::print("\n");
    // 保存法线图像文件
    normalMap->write();
}

bool a3NormalMapRenderer::check()
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

    if(!normalMap)
    {
        a3Log::error("a3SamplerRenderer::render()前normalMap: %d尚未分配指定\n", normalMap);
        return false;
    }
    else
    {
        a3Log::debug("Generating Normal map, width:%d, height:%d\n", normalMap->width, normalMap->height);
    }

    return true;
}

t3Vector3f a3NormalMapRenderer::getNormal(const a3Scene* scene, a3Ray* ray, const a3CameraSample* sample)
{
    a3Intersection intersection;

    if(!scene->intersect(*ray, &intersection))
        // zero normal
        return t3Vector3f(0, 0, 0);

    a3Shape* obj = intersection.shape;

    // 找到相交点
    t3Vector3f intersectPoint = (*ray)(intersection.t);

    return intersection.getNormal();
}
