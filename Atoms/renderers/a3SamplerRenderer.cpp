#include <renderers/a3SamplerRenderer.h>
#include <samples/a3Sampler.h>
#include <core/a3Ray.h>
#include <image/a3Image.h>
#include <cameras/a3PerspectiveCamera.h>
#include <samples/a3CameraSample.h>
#include <core/a3Scene.h>
#include <core/a3Intersection.h>
#include <core/log/a3Log.h>
#include <core/a3Random.h>
#include <shapes/a3Shape.h>

#define DEPTH 5
#define SPP 128

a3Random random;

// ���ȷֲ��������
// ��sinPhi^2 + cosPhi^2����������ȷֲ�����ͶӰ������, u1 u2����������������(������ָ��sin/cos), ���ؽ����Ϊ���������������
// �˴�u1:cosTheta u2:����ռ��һԲ�ܵĽǶȰٷֱ�
t3Vector3f hemisphere(float u1, float u2)
{
    // 1 - cosThta^2 = sinTheta
    const float r = t3Math::sqrt(1.0f - u1 * u1);
    
    // phi = 2Pi * percent
    const float phi = 2 * T3MATH_PI * u2;

    // x = sinTheta * cosPhi; y = sinTheta * sinPhi; z = cosTheta
    return t3Vector3f(t3Math::cosRad(phi) * r, t3Math::sinRad(phi) * r, u1);
}

a3SamplerRenderer::a3SamplerRenderer()
{
}

a3SamplerRenderer::~a3SamplerRenderer()
{

}
// pbrt������Ⱦ���ж����������Ⱦ����
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
        a3Log::error("a3SamplerRenderer::render()ǰcamera: %d�ڴ���δ����", camera);
        return;
    }

    // spp��ʱ�ر�
    sampler = new a3Sampler(0, 0, camera->image->width, camera->image->height, 0);

#pragma omp parallel for schedule(dynamic)
    for(int x = 0; x < camera->image->width; x++)
    {
        a3Log::info("Rendering: %8.2f %%\r", (double) x / camera->image->width * 100);

        for(int y = 0; y < camera->image->height; y++)
        {
            t3Vector3f color;

            // ��ǰ����λ��
            a3CameraSample sample;

            // ��ȡ��һ������λ��
            sampler->getSample(x, y, &sample);

            for(int z = 0; z < SPP; z++)
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

                // �ݲ����ù���ת��
                color = color + Li(scene, &ray, 0, &sample, &intersection) * (1.0f / SPP);
            }

            t3Math::clamp(color.x, 0.0f, 255.0f);
            t3Math::clamp(color.y, 0.0f, 255.0f);
            t3Math::clamp(color.z, 0.0f, 255.0f);

            camera->image->addSample(&sample, color);
        }
    }
    a3Log::info("\n");

    // ����ͼ���ļ�
    camera->image->write();
}

t3Vector3f a3SamplerRenderer::Li(const a3Scene* scene, const a3Ray* ray, int depth, const a3CameraSample* sample, a3Intersection* intersection)
{
    // �ݲ�����Russian Roulette
    if(++depth > DEPTH)
        return t3Vector3f(0, 0, 0);

    if(!scene->intersect(*ray, intersection))
        // black
        return t3Vector3f(0, 0, 0);

    //a3Log::debug("Intersection: x:%f y:%f\n", sample->imageX, sample->imageY);

    a3Shape* obj = scene->objects[intersection->shapeID];

    // �Ӵ���
    t3Vector3f intersectPoint = (*ray)(intersection->t);

    if(obj->type == A3_MATERIAL_DIFFUSS)
    {
        // Diffuse BRDF
        t3Vector3f sampleDirection = hemisphere(random.randomFloat(), random.randomFloat());

        return obj->emission + Li(scene, &a3Ray(intersectPoint, sampleDirection.normalize()), depth, sample, intersection);
    }
    else if(obj->type == A3_MATERIAL_SPECULAR)
    {
        // �ٶ����и���obj����õ�normal��Ϊ��������
        t3Vector3f normal = obj->getNormal(intersectPoint);

        t3Vector3f sampleDirection = ray->direction - 2 * (ray->direction * normal) * normal;

        return obj->emission + Li(scene, &a3Ray(intersectPoint, sampleDirection.normalize()), depth, sample, intersection);
    }
}
