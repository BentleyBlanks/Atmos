#include <renderers/a3SamplerRenderer.h>
#include <samples/a3Sampler.h>
#include <core/a3Ray.h>
#include <image/a3Image.h>
#include <cameras/a3PerspectiveCamera.h>
#include <samples/a3CameraSample.h>
#include <core/a3Scene.h>
#include <core/a3Intersection.h>
#include <core/log/a3Log.h>

a3SamplerRenderer::a3SamplerRenderer()
{
}

a3SamplerRenderer::~a3SamplerRenderer()
{

}
// pbrt划分渲染队列独立任务的渲染管线
// |Task.run()| |Task.run()| ... |Task.run()|
//      sample = RandomSampler.getSubSampler()
//      ray = Camera.generateRayDiffererntial(sample)
//      intersection = Scene.Intersect(ray)
//      spectrum = Intergrator.Li(intersection)
//      File.AddSample(spectrum, sample)
//      Reporter.Update()
void a3SamplerRenderer::render(const a3Scene* scene)
{
    if(!camera)
    {
        a3Log::error("a3SamplerRenderer::render()前camera: %d内存尚未分配", camera);
        return;
    }

    // spp暂时关闭
    sampler = new a3Sampler(0, 0, camera->image->width, camera->image->height, 0);

    for(size_t x = 0; x < camera->image->width; x++)
    {
        a3Log::info("Rendering: %8.2f %%\r", (double) x / camera->image->width * 100);

        for(size_t y = 0; y < camera->image->height; y++)
        {
            // memory allocating
            a3Ray ray;

            a3CameraSample sample;
            
            a3Intersection intersection;

            // 获取下一个采样位置
            sampler->getSample(x, y, &sample);

            // 生成光线
            camera->castRay(&sample, &ray);

            // 暂不启用光谱转换
            t3Vector3f color = Li(scene, &ray, &sample, &intersection);

            camera->image->addSample(&sample, color);
        }
    }
    a3Log::info("\n");

    // 保存图像文件
    camera->image->write();
}

t3Vector3f a3SamplerRenderer::Li(const a3Scene* scene, const a3Ray* ray, const a3CameraSample* sample, a3Intersection* intersection)
{
    if(!scene->intersect(*ray, intersection))
        // black
        return t3Vector3f(0, 0, 0);

    a3Log::debug("Intersection: x:%f y:%f\n", sample->imageX, sample->imageY);

    return t3Vector3f(255, 0, 0);
}
