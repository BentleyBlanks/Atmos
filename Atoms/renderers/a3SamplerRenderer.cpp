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

a3SamplerRenderer::a3SamplerRenderer() : spp(2000), bounces(10), sampler(NULL), camera(NULL), enableGammaCorrection(true)
{

}

a3SamplerRenderer::~a3SamplerRenderer()
{
        
}
// pbrt������Ⱦ���ж����������Ⱦ����
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
        a3Log::error("a3SamplerRenderer::render()ǰcamera: %d�ڴ���δ����", camera);
        return;
    }

    // spp��ʱ�ر�
    sampler = new a3Sampler(0, 0, camera->image->width, camera->image->height, 0);

#ifdef A3_RENDERING_NORMALMAP
    // ��Ⱦ������ͼ���ڼ�ⷨ��
#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < camera->image->width; x++)
    {
        a3Log::info("Normal Map    Rendering: %8.2f \r", (double) x / camera->image->width * 100);

        for(int y = 0; y < camera->image->height; y++)
        {
            // ��ǰ����λ��
            a3CameraSample sample; 
            a3Ray ray;

            // ��ȡ��һ������λ��
            sampler->getSample(x, y, &sample);

            camera->castRay(&sample, &ray);

            t3Vector3f n = getNormal(scene, &ray, &sample);

            camera->normalMap->addSample(&sample, n);
        }
    }

    a3Log::print("\n");
    // ���淨��ͼ���ļ�
    camera->normalMap->write();
#endif

#ifdef A3_RENDERING_REALISTICIMAGE
#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < camera->image->width; x++)
    {
        a3Log::info("Spp:%d, depth:%d    Rendering: %8.2f \r", spp, bounces, (double) x / camera->image->width * 100);

        for(int y = 0; y < camera->image->height; y++)
        {
            t3Vector3f color;

            // ��ǰ����λ��
            a3CameraSample sample;

            // ��ȡ��һ������λ��
            sampler->getSample(x, y, &sample);

            for(int z = 0; z < spp; z++)
            {
                // memory allocating
                a3Ray ray;

                a3Intersection intersection;

                // ��ԭ�в���λ�ô�[-1, 1]���������
                a3CameraSample sampleTentFilter;

                sampleTentFilter.imageX = sample.imageX + random.randomFloat() * 2 - 1;
                sampleTentFilter.imageY = sample.imageY + random.randomFloat() * 2 - 1;

                // ���ɹ���
                camera->castRay(&sampleTentFilter, &ray);

                t3Vector3f temp;
                // �ݲ����ù���ת��
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

    // ������ʵ��Ⱦͼ���ļ�
    camera->image->write();
#endif
}

void a3SamplerRenderer::Li(const a3Scene* scene, const a3Ray* ray, int depth, t3Vector3f& color, const a3CameraSample* sample, a3Intersection* intersection)
{
    if(++depth > bounces)
    {
        color = t3Vector3f::zero();
        return;
    }

    // �ݲ�����Russian Roulette
    if(!scene->intersect(*ray, intersection))
    {
        // �������δ�����κι�Դ���(��������Զ�����ʵ�ִ˷���)
        t3Vector3f Le;
        for(auto l : scene->lights)
            Le += l->Le(*ray);

        color += Le;

        return;
    }

    a3Shape* obj = scene->objects[intersection->shapeID];

    // �Ӵ���
    t3Vector3f intersectPoint = (*ray)(intersection->t);

    // �Է���ϵ��
    color += obj->emission;

    // �ٶ����и���obj����õ�normal��Ϊ��������
    t3Vector3f normal = obj->getNormal(intersectPoint);

    if(obj->type == A3_MATERIAL_DIFFUSS)
    {
        // Diffuse BRDF
        t3Vector3f rotatedX, rotatedY;
        a3OrthonomalSystem(normal, rotatedX, rotatedY);

        t3Vector3f sampleDirection = a3Hemisphere(random.randomFloat(), random.randomFloat());
        sampleDirection.normalize();

        // sampleDirectionת����Normal����ϵ��
        t3Vector3f wo;
        wo.x = t3Vector3f(rotatedX.x, rotatedY.x, normal.x).dot(sampleDirection);
        wo.y = t3Vector3f(rotatedX.y, rotatedY.y, normal.y).dot(sampleDirection);
        wo.z = t3Vector3f(rotatedX.z, rotatedY.z, normal.z).dot(sampleDirection);

        a3Ray reflectRay(intersectPoint, wo);

        float cosTheta = ray->direction.dot(normal);

        t3Vector3f radiance;
        Li(scene, &reflectRay, depth, radiance, sample, intersection);
        color += radiance * obj->color;
    }
    else if(obj->type == A3_MATERIAL_SPECULAR)
    {
        t3Vector3f wo = (ray->direction - 2 * (normal.dot(ray->direction)) * normal).normalize();

        a3Ray reflectRay(intersectPoint, wo);

        t3Vector3f radiance;
        Li(scene, &reflectRay, depth, radiance, sample, intersection);
        color += radiance * obj->color;
    }
    else if(obj->type == A3_METERIAL_REFRACTION)
    {
        a3Ray reflectRay, transmittedRay;

        float cosTheta1 = ray->direction.dot(normal);

        // �Ƿ��ڹ��ܽ����ڲ�
        bool into = cosTheta1 < 0;

        // n = ��������ڲ��������� / �������������
        float n1 = 1.0f, n2 = obj->refractiveIndex;
        float n = into ? n1 / n2 : n2 / n1;

        float cosTheta2 = 1 - n * n * (1 - cosTheta1 * cosTheta1);

        // ȫ����
        if(cosTheta2 < 0)
        {
            t3Vector3f totalReflec = (ray->direction - normal * 2 * (ray->direction.dot(normal))).normalize();
            reflectRay.set(intersectPoint, totalReflec);

            t3Vector3f radiance;
            Li(scene, &reflectRay, depth, radiance, sample, intersection);
            color += radiance * obj->color;
            return;
        }

        // Schlick�Ľ��Ʒ���
        float R0 = (1.0f - n) / (1.0 + n);
        R0 = R0 * R0;

        
        // �����뷴����ʿ��� ͨ��[0, 1)֮����������� ���ݴ�С���������䷴��ѡ��
        // --!����ɼ� http://www.kevinbeason.com/smallpt/
        t3Vector3f wo, wr;

        wo = n * (ray->direction - normal * cosTheta1) - (into ? normal : -normal) * t3Math::sqrt(cosTheta2);
        
        // ������߷���
        wr = ray->direction - 2 * (ray->direction.dot(normal)) * normal;

        // ��������ĸ���
        float probablity = R0 + (1 - R0) * t3Math::pow((1 - t3Math::Abs(cosTheta1)), 5);

        t3Vector3f radianceT, radianceR;

        //if(random.randomFloat() > probablity)
        //{
            transmittedRay.set(intersectPoint, wo.normalize());
            Li(scene, &transmittedRay, depth, radianceT, sample, intersection);
        //}
        //else
        //{
            reflectRay.set(intersectPoint, wr.normalize());
            Li(scene, &reflectRay, depth, radianceR, sample, intersection);
        //}
        //color += radianceT * obj->color;

        //if(depth > 2)
        //{
        //    float P = 0.25 + 0.5 * probablity, RP = probablity / P, TP = (1 - probablity) / (1 - P);
        //    // ��������
        //    color += (radianceT * RP + radianceR * TP) * obj->color;
        //}
        //else
            color += (radianceR * probablity + radianceT * (1 - probablity)) * obj->color;
    }
}

t3Vector3f a3SamplerRenderer::getNormal(const a3Scene* scene, a3Ray* ray, const a3CameraSample* sample)
{
    a3Intersection intersection;

    if(!scene->intersect(*ray, &intersection))
        // zero normal
        return t3Vector3f(0, 0, 0);

    a3Shape* obj = scene->objects[intersection.shapeID];

    // �Ӵ���
    t3Vector3f intersectPoint = (*ray)(intersection.t);

    return obj->getNormal(intersectPoint);
}