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
    A3_MATERIAL_NONE = 0,
    A3_MATERIAL_DIFFUSS = 1,
    A3_MATERIAL_SPECULAR = 2,
    A3_METERIAL_REFRACTION = 3
};

enum a3SceneName
{
    WENDAOQIUER = 0,
    INFINITEAREA_LIGHT = 1,
    CORNEL_BOX = 2,
    BVHTEST = 3
};

enum a3RendererName
{
    SINGLERAY = 0,
    NORMALMAP = 1,
    SAMPLER = 2
};

enum a3IntegratorName
{
    PATH = 0,
    DIRECT_LIGHTING = 1
};

enum a3PrimitiveSetName
{
    EXHAUSTIVE = 0,
    BVH = 1
};

// global config
int singleX = 264, singleY = 173;
int spp = 256;
int maxDepth = 8;
int imageWidth = 700, imageHeight = 700;

a3SceneName name = BVHTEST;
a3RendererName rendererName = SAMPLER;
a3IntegratorName integratorName = PATH;
a3PrimitiveSetName primitiveName = BVH;

inline a3PerspectiveSensor* generateCamera(a3Film* image, a3SceneName name)
{
    a3PerspectiveSensor* camera = NULL;

    if(name == INFINITEAREA_LIGHT)
        camera = new a3PerspectiveSensor(t3Vector3f(0, 50, -100), t3Vector3f(0, 0, 20), t3Vector3f(0, 1, 0), 40, 210.0f, 0.0f, image);
    else if(name == WENDAOQIUER)
        camera = new a3PerspectiveSensor(t3Vector3f(0, -150, -10), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 55.517f, 210.0f, 0.0f, image);
    else if(name == CORNEL_BOX)
        camera = new a3PerspectiveSensor(t3Vector3f(50.0f, 52.0f, 295.6f), t3Vector3f(50.0f, 52.0f - 0.042612f, 295.6f - 1.0f), t3Vector3f(0, 1, 0), 40.0f, 210.0f, 0.0f, image);
    else if(name == BVHTEST)
        camera = new a3PerspectiveSensor(t3Vector3f(0, 100, 50), t3Vector3f(0, 0, 10), t3Vector3f(0, 0, 1), 40, 210.0f, 0.0f, image);
    
    camera->print();

    return camera;
}

inline a3Renderer* gengerateRenderer(a3PerspectiveSensor* camera, a3Film* image, a3RendererName name, a3IntegratorName integratorName)
{
    a3Renderer* renderer = NULL;

    if(name == SINGLERAY)
    {
        a3SingleRayRenderer* r = new a3SingleRayRenderer(singleX, singleY);

        r->sampler = new a3RandomSampler();
        r->camera = camera;

        renderer = r;
    }
    else if(name == NORMALMAP)
    {
        a3NormalMapRenderer* r = new a3NormalMapRenderer();

        r->normalMap = new a3NormalMap(*image);
        r->normalMap->setFileName("../../../../resources/results/normalMap");
        r->sampler = new a3RandomSampler();
        r->camera = camera;

        renderer = r;
    }
    else if(name == SAMPLER)
    {
        a3SamplerRenderer* r = new a3SamplerRenderer(spp);

        r->enableGammaCorrection = false;
        r->sampler = new a3RandomSampler();
        r->camera = camera;

        renderer = r;
    }

    if(name == SINGLERAY || name == SAMPLER)
    {
        a3SamplerRenderer* r = (a3SamplerRenderer*)renderer;

        if(integratorName == PATH)
            r->integrator = new a3PathIntegrator(maxDepth);
        else if(integratorName == DIRECT_LIGHTING)
            r->integrator = new a3DirectLightingIntegrator();
    }
    
    return renderer;
}

inline a3Scene* generateScene(a3SceneName name, a3PrimitiveSetName primitiveName)
{
    a3BVH* bvh = NULL;
    a3Scene* scene = new a3Scene();

    if(primitiveName == EXHAUSTIVE)
        scene->primitiveSet = new a3Exhaustive();
    else if(primitiveName == BVH)
        scene->primitiveSet = bvh = new a3BVH();

    auto addShape = [&scene](a3Shape* s, a3Spectrum R, a3Spectrum emission, int type, a3Texture<a3Spectrum>* texture = NULL)->auto
    {
        s->emission = emission;

        switch(type)
        {
        case A3_MATERIAL_DIFFUSS:
            s->bsdf = new a3Diffuse(R);
            break;
        case A3_MATERIAL_SPECULAR:
            s->bsdf = new a3Conductor(R);
            break;
        case A3_METERIAL_REFRACTION:
            s->bsdf = new a3Dieletric(R);
            break;
        default:
            break;
        }

        s->bsdf->texture = texture;

        scene->addShape(s);

        return s->bsdf;
    };

    if(name == INFINITEAREA_LIGHT)
    {
        //scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/skylightSurreal.png"));
        scene->addLight(new a3PointLight(t3Vector3f(0, 10, -100), a3Spectrum(500000.0f)));

        a3ModelImporter importer;
        //std::vector<a3Shape*>* shapes = importer.load("../../../../resources/models/spheres.obj");
        std::vector<a3Shape*>* shapes = importer.load("../../../../resources/models/spheres.obj");

        if(shapes)
        {
            for(auto s : *shapes)
                addShape(s, t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

            //addShape(new a3Sphere(t3Vector3f(-40, -60, 25), 25), t3Vector3f(1.0f, 1.0f, 1.0f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
            //addShape(new a3Sphere(t3Vector3f(40, 30, 25), 25), t3Vector3f(1.0f, 1.0f, 1.0f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);

            //addShape(new a3Plane(t3Vector3f(0, 0, -20), t3Vector3f(0, 0, 1)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
            addShape(new a3Disk(t3Vector3f(0, 0, 0), 1000, t3Vector3f(0, 1, 0)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
        }
    }
    else if(name == WENDAOQIUER)
    {
        scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/pisaLatlong.png"));

        addShape(new a3Sphere(t3Vector3f(-30, -8, 0), 37), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

        // 无限远平面
        addShape(new a3Plane(t3Vector3f(0, 0, -37), t3Vector3f(0, 0, 1)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    }
    else if(name == CORNEL_BOX)
    {
        // Spheres  
        addShape(new a3Sphere(t3Vector3f(73.0f, 16.5f, 47.0f), 16.5f), t3Vector3f(1.0f, 1.0f, 1.0f), t3Vector3f(0.0f, 0.0f, 0.0f), A3_MATERIAL_SPECULAR);
        addShape(new a3Sphere(t3Vector3f(27.0f, 16.5f, 78.0f), 16.5f), t3Vector3f(1.0f, 1.0f, 1.0f), t3Vector3f(0.0f, 0.0f, 0.0f), A3_METERIAL_REFRACTION);

        // Plane
        addShape(new a3Plane(t3Vector3f(1.0f, 40.8f, 81.6f), t3Vector3f(1.0f, 0.0f, 0.0f)), t3Vector3f(0.25f, 0.25f, 0.75f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
        addShape(new a3Plane(t3Vector3f(99.0f, 40.8f, 81.6f), t3Vector3f(-1.0f, 0.0f, 0.0f)), t3Vector3f(0.75f, 0.25f, 0.25f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
        addShape(new a3Plane(t3Vector3f(50.0f, 40.8f, 0.0f), t3Vector3f(0.0f, 0.0f, 1.0f)), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
        //addShape(new a3Plane(t3Vector3f(50.0f, 40.8f, 310.0f), t3Vector3f(0.0f, 0.0f, 1)), t3Vector3f(0.0f, 0.0f, 0.0f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
        addShape(new a3Plane(t3Vector3f(50.0f, 0.0f, 81.6f), t3Vector3f(0.0f, 1.0f, 0.0f)), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
        addShape(new a3Plane(t3Vector3f(50.0f, 81.6f, 81.6f), t3Vector3f(0.0f, -1.0f, 0.0f)), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);

        // 光源
        //addShape(new a3Sphere(t3Vector3f(50.0f, 681.6f - 0.27f, 81.6f), 600.0f), t3Vector3f(0.0f, 0.0f, 0.0f), t3Vector3f(5000.0f, 5000.0f, 5000.0f), A3_MATERIAL_DIFFUSS);
        //scene->addLight(new a3PointLight(t3Vector3f(50.0f, 81.6f - 0.27f, 81.6f), a3Spectrum(200000.0f)));
        scene->addLight(new a3SpotLight(t3Vector3f(50.0f, 81.6f - 0.27f, 81.6f), t3Vector3f(0.0f, -1.0f, 0.0f), a3Spectrum(1000000.0f), 40, 5));
    }
    else if(name == BVHTEST)
    {
        scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/Mitsuba.png"));

        a3ModelImporter importer;
        std::vector<a3Shape*>* plane = importer.load("../../../../resources/models/mitsuba/mitsuba_plane.obj");
        std::vector<a3Shape*>* internal = importer.load("../../../../resources/models/mitsuba/mitsuba_internal.obj");
        std::vector<a3Shape*>* sphere = importer.load("../../../../resources/models/mitsuba/mitsuba_sphere.obj");

        a3BSDF* bsdf;
        a3CheckerBoard<a3Spectrum>* texture = a3CreateChekerBoardTexture();
        //a3ImageTexture<a3Spectrum>* texture = a3CreateImageTexture("../../../../resources/images/wood2.png");

        // plane
        if(plane)
        {
            for(auto s : *plane)
            {
                bsdf = addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
                bsdf->texture = texture;
            }
        }

        // internal
        if(internal)
        {
            for(auto s : *internal)
            {
                bsdf = addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
                //bsdf->texture = texture;
            }
        }

        // sphere
        if(sphere)
        {
            for(auto s : *sphere)
            {
                bsdf = addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
                //bsdf->texture = texture;
            }
        }
    }

    // 加速结构初始化
    if(primitiveName == BVH)
        bvh->init();

    return scene;
}

int main()
{
    // alloc
    a3Film* image = new a3Film(imageWidth, imageHeight, "../../../../resources/results/hello", A3_IMAGE_PNG);

    a3PerspectiveSensor* camera = generateCamera(image, name);

    a3Scene* scene = generateScene(name, primitiveName);

    a3Renderer* renderer = gengerateRenderer(camera, image, rendererName, integratorName);

    // rendering
    t3Timer timer;
    timer.start();

    renderer->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
    return 0;
} 