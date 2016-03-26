#include <renderers/a3SamplerRenderer.h>
#include <samples/a3Sampler.h>
#include <core/a3Ray.h>
#include <core/image/a3Film.h>
#include <cameras/a3PerspectiveCamera.h>
#include <samples/a3CameraSample.h>
#include <core/a3Scene.h>
#include <core/a3Intersection.h>
#include <core/log/a3Log.h>
#include <shapes/a3Shape.h>
#include <core/a3Warp.h>
#include <lights/a3Light.h>
#include <core/a3Random.h>
#include <core/image/a3NormalMap.h>

a3Random random;

//#define A3_RENDERING_NORMALMAP
#define A3_RENDERING_REALISTICIMAGE

a3SamplerRenderer::a3SamplerRenderer() : spp(128), bounces(7), sampler(NULL), camera(NULL), enableGammaCorrection(false)
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
    if(!camera)
    {
        a3Log::error("a3SamplerRenderer::render()前camera: %d内存尚未分配", camera);
        return;
    }

    // spp暂时关闭
    sampler = new a3Sampler(0, 0, camera->image->width, camera->image->height, 0);

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
            t3Vector3f color;

            // 当前采样位置
            a3CameraSample sample;

            // 获取下一个采样位置
            sampler->getSample(x, y, &sample);

            for(int z = 0; z < spp; z++)
            {
                // memory allocating
                a3Ray ray;

                a3Intersection intersection;

                // 在原有采样位置处[-1, 1]做随机采样
                a3CameraSample sampleTentFilter;

                sampleTentFilter.imageX = sample.imageX + random.randomFloat() * 2 - 1;
                sampleTentFilter.imageY = sample.imageY + random.randomFloat() * 2 - 1;

                // 生成光线
                camera->castRay(&sampleTentFilter, &ray);

                t3Vector3f temp;
                // 暂不启用光谱转换
                Li(scene, &ray, 0, temp, &sample, &intersection);

                color += temp / spp;
            }

            color.x = t3Math::clamp(color.x, 0.0f, 255.0f);
            color.y = t3Math::clamp(color.y, 0.0f, 255.0f);
            color.z = t3Math::clamp(color.z, 0.0f, 255.0f);

            //t3Vector3f toneColor = color;
            //t3Vector3f color = a3Tonemap(color / 255.0f);

            if(enableGammaCorrection)
                a3GammaCorrection(color);

            camera->image->addSample(&sample, color);
        }
    }
    a3Log::print("\n");

    // 保存真实渲染图像文件
    camera->image->write();
#endif
}

void a3SamplerRenderer::Li(const a3Scene* scene, a3Ray* ray, int depth, t3Vector3f& color, const a3CameraSample* sample, a3Intersection* intersection)
{
    if(++depth > bounces)
    {
        color = t3Vector3f::zero();
        return;
    }

    // 暂不启用Russian Roulette
    if(!scene->intersect(*ray, intersection))
    {
        // 处理光线未击中任何光源情况(仅有无限远区域光实现此方法)
        t3Vector3f Le;
        for(auto l : scene->lights)
            Le += l->Le(*ray);

        color += Le;

        return;
    }

    a3Shape* obj = scene->objects[intersection->shapeID];

    // 接触点
    t3Vector3f intersectPoint = (*ray)(intersection->t);

    // 自发光系数
    color += obj->emission;

    if(obj->type == A3_MATERIAL_DIFFUSS)
    {
        // Diffuse BRDF
        t3Vector3f normal = obj->getNormal(intersectPoint), rotatedX, rotatedY;
        a3OrthonomalSystem(normal, rotatedX, rotatedY);

        t3Vector3f sampleDirection = a3Hemisphere(random.randomFloat(), random.randomFloat());
        sampleDirection.normalize();

        // sampleDirection转换到Normal坐标系下
        t3Vector3f rotatedDirection;
        rotatedDirection.x = t3Vector3f(rotatedX.x, rotatedY.x, normal.x).dot(sampleDirection);
        rotatedDirection.y = t3Vector3f(rotatedX.y, rotatedY.y, normal.y).dot(sampleDirection);
        rotatedDirection.z = t3Vector3f(rotatedX.z, rotatedY.z, normal.z).dot(sampleDirection);

        ray->set(intersectPoint, rotatedDirection);

        float cosTheta = ray->direction.dot(normal);

        t3Vector3f temp;
        Li(scene, ray, depth, temp, sample, intersection);
        //if(temp != t3Vector3f::zero())
        //    printf("dwdw");

        //color += (temp * obj->color) * cosTheta * 0.1;
        color += (temp * obj->color) * 0.9;
    }
    else if(obj->type == A3_MATERIAL_SPECULAR)
    {
        // 假定所有给定obj上求得的normal都为方向向量
        t3Vector3f normal = obj->getNormal(intersectPoint);

        t3Vector3f sampleDirection = ray->direction - 2 * (ray->direction.dot(normal)) * normal;

        ray->set(intersectPoint, sampleDirection.normalize());

        t3Vector3f temp;
        Li(scene, ray, depth, temp, sample, intersection);
        color += temp;
    }
    else if(obj->type == A3_METERIAL_REFRACTION)
    {
        // 折射与反射概率可求 通过[0, 1)之间生成随机数 根据大小间接完成折射反射选择
        // 一次迭代只进行折射或反射迭代
        // n = n2 / n1 假定在介质外 n1 = 1
        float n = obj->refractiveIndex;

        // Schlick的近似方程
        float R0 = (1.0f - n) / (1.0 + n);
        R0 = R0 * R0;

        t3Vector3f normal = obj->getNormal(intersectPoint);

        // 出射方向取决于当前光线位置是否处于物体中
        if(normal.dot(ray->direction) > 0)
        {
            // 处于介质当中 反转表面法线方向
            normal = -normal;
            // n = n2 / n1 介质内 n2 = 1
            n = 1 / n;
        }

        n = 1 / n;

        // 入射角(光疏到光密: cosTheta1 < 0，而角度[0, 90]，需调制*-1; 光密到光疏: cosTheta1 > 0, 而normal被反转, 需调制*-1)
        float cosTheta1 = normal.dot(ray->direction) * -1;
        // 折射角(cosTheta2^2) 
        float cosTheta2 = 1 - n * n * (1 - cosTheta1 * cosTheta1);

        // 发生反射的概率
        float probablity = R0 + (1 - R0)*t3Math::pow((1 - cosTheta1), 5);

        t3Vector3f outDirection;
        // 折射光线方向(且未发生全反射)
        if(cosTheta2 > 0 && random.randomFloat() > probablity)
            outDirection = ray->direction * n + normal * (n * cosTheta1 - t3Math::sqrt(cosTheta2));
        // 反射光线方向
        else
            // d - 2*(l · N)·N; l · N = -cosTheta1; 
            outDirection = ray->direction + normal * (2 * cosTheta1);

        ray->set(intersectPoint, outDirection.normalize());

        t3Vector3f temp;
        Li(scene, ray, depth, temp, sample, intersection);
        color += temp;
    }
}

t3Vector3f a3SamplerRenderer::getNormal(const a3Scene* scene, a3Ray* ray, const a3CameraSample* sample)
{
    a3Intersection intersection;

    if(!scene->intersect(*ray, &intersection))
        // zero normal
        return t3Vector3f(0, 0, 0);

    a3Shape* obj = scene->objects[intersection.shapeID];

    // 接触点
    t3Vector3f intersectPoint = (*ray)(intersection.t);

    return obj->getNormal(intersectPoint);
}
