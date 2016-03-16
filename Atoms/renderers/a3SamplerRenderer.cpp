#include <renderers/a3SamplerRenderer.h>
#include <samples/a3Sampler.h>
#include <core/a3Ray.h>
#include <core/image/a3Film.h>
#include <cameras/a3PerspectiveCamera.h>
#include <samples/a3CameraSample.h>
#include <core/a3Scene.h>
#include <core/a3Intersection.h>
#include <core/log/a3Log.h>
#include <core/a3Random.h>
#include <shapes/a3Shape.h>
#include <core/a3Warp.h>
#include <lights/a3Light.h>

#define A3_RANDOM_SAMPLING

#ifdef A3_RANDOM_SAMPLING
#define SPP 2048
#define DEPTH 7

#else
#define SPP 1
#define DEPTH 1
#endif

//#define A3_GAMMA_CORRECTION

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

void ons(const t3Vector3f& v1, t3Vector3f& v2, t3Vector3f& v3)
{
    if(std::abs(v1.x) > std::abs(v1.y))
    {
        // project to the y = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
        v2 = t3Vector3f(-v1.z * invLen, 0.0f, v1.x * invLen);
    }
    else
    {
        // project to the x = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
        v2 = t3Vector3f(0.0f, v1.z * invLen, -v1.y * invLen);
    }

    v3 = v1.getCrossed(v2);
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
        a3Log::info("Spp:%d    Rendering: %8.2f \r", SPP, (double) x / camera->image->width * 100);

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

            color.x = t3Math::clamp(color.x, 0.0f, 255.0f);
            color.y = t3Math::clamp(color.y, 0.0f, 255.0f);
            color.z = t3Math::clamp(color.z, 0.0f, 255.0f);
            
#ifdef A3_GAMMA_CORRECTION
            t3Vector3f toneColor = color;
            //t3Vector3f toneColor = a3Tonemap(color / 255.0f);

            //toneColor.x = t3Math::pow(toneColor.x, 2.2);
            //toneColor.y = t3Math::pow(toneColor.y, 2.2);
            //toneColor.z = t3Math::pow(toneColor.z, 2.2);

            toneColor.x = t3Math::pow(toneColor.x / 255.0f, 1 / 2.2f);
            toneColor.y = t3Math::pow(toneColor.y / 255.0f, 1 / 2.2f);
            toneColor.z = t3Math::pow(toneColor.z / 255.0f, 1 / 2.2f);

            toneColor *= 255;

            camera->image->addSample(&sample, toneColor);
#else
            camera->image->addSample(&sample, color);
#endif
        }
    }
    a3Log::print("\n");

    // ����ͼ���ļ�
    camera->image->write();
}

t3Vector3f a3SamplerRenderer::Li(const a3Scene* scene, a3Ray* ray, int depth, const a3CameraSample* sample, a3Intersection* intersection)
{
    // �ݲ�����Russian Roulette
    if(++depth > DEPTH)
        return t3Vector3f(0, 0, 0);

    if(!scene->intersect(*ray, intersection))
    {
        //// black
        //return t3Vector3f(0, 0, 0);

        // �������δ�����κι�Դ���(��������Զ�����ʵ�ִ˷���)
        t3Vector3f Le;
        for(auto l : scene->lights)
            Le += l->Le(*ray);

        return Le;
    }

    //a3Log::debug("Intersection: x:%f y:%f\n", sample->imageX, sample->imageY);

    a3Shape* obj = scene->objects[intersection->shapeID];

    // �Ӵ���
    t3Vector3f intersectPoint = (*ray)(intersection->t);

#ifdef A3_RANDOM_SAMPLING
    if(obj->type == A3_MATERIAL_DIFFUSS)
    {
        // Diffuse BRDF
        t3Vector3f sampleDirection = hemisphere(random.randomFloat(), random.randomFloat());
        sampleDirection.normalize();

        ray->set(intersectPoint, sampleDirection);

        float cosTheta = ray->direction.dot(obj->getNormal(intersectPoint));
        
        return obj->emission + Li(scene, ray, depth, sample, intersection);
    }
    else if(obj->type == A3_MATERIAL_SPECULAR)
    {
        // �ٶ����и���obj����õ�normal��Ϊ��������
        t3Vector3f normal = obj->getNormal(intersectPoint);

        t3Vector3f sampleDirection = ray->direction - 2 * (ray->direction.dot(normal)) * normal;

        ray->set(intersectPoint, sampleDirection.normalize());

        return obj->emission + Li(scene, ray, depth, sample, intersection);
    }
    else if(obj->type == A3_METERIAL_REFRACTION)
    {
        // �����뷴����ʿ��� ͨ��[0, 1)֮����������� ���ݴ�С���������䷴��ѡ��
        // һ�ε���ֻ��������������
        // n = n2 / n1 �ٶ��ڽ����� n1 = 1
        float n = obj->refractiveIndex;

        // Schlick�Ľ��Ʒ���
        float R0 = (1.0f - n) / (1.0 + n);
        R0 = R0 * R0;

        t3Vector3f normal = obj->getNormal(intersectPoint);

        // ���䷽��ȡ���ڵ�ǰ����λ���Ƿ���������
        if(normal.dot(ray->direction) > 0)
        {
            // ���ڽ��ʵ��� ��ת���淨�߷���
            normal = -normal;
            // n = n2 / n1 ������ n2 = 1
            n = 1 / n;
        }

        n = 1 / n;

        // �����(���赽����: cosTheta1 < 0�����Ƕ�[0, 90]�������*-1; ���ܵ�����: cosTheta1 > 0, ��normal����ת, �����*-1)
        float cosTheta1 = normal.dot(ray->direction) * -1;
        // �����(cosTheta2^2) 
        float cosTheta2 = 1 - n * n * (1 - cosTheta1 * cosTheta1);

        // ��������ĸ���
        float probablity = R0 + (1 - R0)*t3Math::pow((1 - cosTheta1), 5);
        
        t3Vector3f outDirection;
        // ������߷���(��δ����ȫ����)
        if(cosTheta2 > 0 && random.randomFloat() > probablity)
            outDirection = ray->direction * n + normal * (n * cosTheta1 - t3Math::sqrt(cosTheta2));
        // ������߷���
        else
            // d - 2*(l �� N)��N; l �� N = -cosTheta1; 
            outDirection = ray->direction + normal * (2 * cosTheta1);
        
        ray->set(intersectPoint, outDirection.normalize());

        return obj->emission + 1.15 * Li(scene, ray, depth, sample, intersection);
    }

#else        
    return obj->emission + obj->color;
#endif
}
