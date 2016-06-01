// shape
#include <shapes/a3Sphere.h>
#include <shapes/a3Plane.h>
#include <shapes/a3Disk.h>
#include <shapes/a3Triangle.h>

#include <Common/t3Timer.h>

// light
#include <lights/a3InfiniteAreaLight.h>
#include <lights/a3PointLight.h>
#include <lights/a3SpotLight.h>

// renderer
#include <renderers/a3SamplerRenderer.h>
#include <renderers/a3NormalMapRenderer.h>
#include <renderers/a3SingleRayRenderer.h>
#include <renderers/a3FresnelRenderer.h>

// textures
#include <textures/a3ImageTexture.h>
#include <textures/a3ConstantTexture.h>
#include <textures/a3CheckerBoard.h>

#include <accelerators/a3BVH.h>
#include <accelerators/a3Exhaustive.h>
#include <samples/a3RandomSampler.h>
#include <sensors/a3PerspectiveSensor.h>

//integrator
#include <integrator/a3PathIntegrator.h>
#include <integrator/a3DirectLighting.h>

// bsdf
#include <bsdf/a3Diffuse.h>
#include <bsdf/a3Conductor.h>
#include <bsdf/a3Dielectric.h>

// core
#include <core/log/a3Log.h>
#include <core/a3ModelImporter.h>
#include <core/a3Common.h>
#include <core/image/a3NormalMap.h>
#include <core/image/a3Film.h>
#include <core/a3Scene.h>

enum a3MaterialType
{
    NONE = 0,
    LAMBERTIAN = 1,
    MIRROR = 2,
    GLASS = 3
};

int main()
{
    // alloc
    a3Film* image = new a3Film(700, 700, "../../../../resources/results/fresnel.png");

    a3PerspectiveSensor* camera = new a3PerspectiveSensor(t3Vector3f(0, 100, 50), t3Vector3f(0, -20, 0), t3Vector3f(0, 0, 1), 40, 100.0f, 0.0f, image);
    a3FresnelRenderer* r = new a3FresnelRenderer();
    r->camera = camera;
    r->image = image;
    r->sampler = new a3RandomSampler();

    a3BVH* bvh = NULL;
    a3Scene* scene = new a3Scene();
    scene->primitiveSet = bvh = new a3BVH();

    auto addShape = [&scene](a3Shape* s, a3Spectrum R, a3Spectrum emission, int type, a3Texture<a3Spectrum>* texture)->auto
    {
        s->emission = emission;

        switch(type)
        {
        case LAMBERTIAN:
            s->bsdf = new a3Diffuse(R);
            break;
        case MIRROR:
            s->bsdf = new a3Conductor(R);
            break;
        case GLASS:
            s->bsdf = new a3Dieletric(R);
            break;
        default:
            break;
        }

        s->bsdf->texture = texture;
        if(texture)
            s->bCalTextureCoordinate = true;

        scene->addShape(s);

        return s->bsdf;
    };

    scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/envmap.exr"));

    a3ModelImporter importer;
    std::vector<a3Shape*>* mitsuba = importer.load("../../../../resources/models/mitsuba.obj");

    if(mitsuba)
        for(auto s : *mitsuba)
            addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), GLASS, NULL);

    bvh->init();

    // rendering
    t3Timer timer;
    timer.start();

    r->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
    return 0;
}