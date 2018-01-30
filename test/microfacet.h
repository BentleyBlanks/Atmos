#include <Atmos.h>

// Renderer
#define SAMPLER
//#define SINGLE_RAY

void main()
{
    // camera
    a3Film* image = new a3Film(768, 512, "../../../../resources/results/bb.png");

    a3PerspectiveSensor* camera = new a3PerspectiveSensor(t3Vector3f(0, 2, 15),
                                                          t3Vector3f(0, -2, 2.5),
                                                          t3Vector3f(0, 1, 0),
                                                          45, 210.0f, 0.0f, image);

    // scene
    a3Scene* scene = new a3Scene();
    scene->primitiveSet = new a3Exhaustive();

    // Matrix
    t3Matrix4x4 sphereM1, sphereM2, sphereM3, sphereM4, sphereM5;
    sphereM1.set(1, 0, 0, 10,
                 0, 1, 0, 10,
                 0, 0, 1, 4,
                 0, 0, 0, 1);

    sphereM2.set(1, 0, 0, -1.25,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);

    sphereM3.set(1, 0, 0, -3.75,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);

    sphereM4.set(1, 0, 0, 1.75,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);

    sphereM5.set(1, 0, 0, 3.75,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);

    // spheres
    a3Sphere* sphere1 = new a3Sphere(sphereM1, 0.5);
    a3Sphere* sphere2 = new a3Sphere(sphereM2, 0.1);
    a3Sphere* sphere3 = new a3Sphere(sphereM3, 0.03333);
    a3Sphere* sphere4 = new a3Sphere(sphereM4, 0.3);
    a3Sphere* sphere5 = new a3Sphere(sphereM5, 0.9);

    // light
    a3AreaLight* l1 = new a3AreaLight(sphere1, a3Spectrum(800), sphereM1);
    a3AreaLight* l2 = new a3AreaLight(sphere2, a3Spectrum(100), sphereM2);
    a3AreaLight* l3 = new a3AreaLight(sphere3, a3Spectrum(901.803), sphereM3);
    a3AreaLight* l4 = new a3AreaLight(sphere4, a3Spectrum(11.1111), sphereM4);
    a3AreaLight* l5 = new a3AreaLight(sphere5, a3Spectrum(1.23457), sphereM5);

    a3ModelImporter importer;
    std::vector<a3Shape*> plate1 = importer.load("../../../../resources/models/MIS/plate1.obj");
    std::vector<a3Shape*> plate2 = importer.load("../../../../resources/models/MIS/plate2.obj");
    std::vector<a3Shape*> plate3 = importer.load("../../../../resources/models/MIS/plate3.obj");
    std::vector<a3Shape*> plate4 = importer.load("../../../../resources/models/MIS/plate4.obj");
    std::vector<a3Shape*> floor = importer.load("../../../../resources/models/MIS/floor.obj");

    // material
    a3Diffuse* diffuse = new a3Diffuse();
    a3Diffuse* floarDiffuss = new a3Diffuse();
    floarDiffuss->R.set(0.4, 0.4, 0.4);

    a3Dielectric* dielectric = new a3Dielectric();

    for(auto s : plate1)
    {
        s->setBSDF(diffuse);
        scene->addShape(s);
    }

    for(auto s : plate2)
    {
        s->setBSDF(diffuse);
        scene->addShape(s);
    }

    for(auto s : plate3)
    {
        s->setBSDF(diffuse);
        scene->addShape(s);
    }

    for(auto s : plate4)
    {
        s->setBSDF(diffuse);
        scene->addShape(s);
    }

    for(auto s : floor)
    {
        s->setBSDF(floarDiffuss);
        scene->addShape(s);
    }

    // scene
    scene->addLight(l1);
    scene->addShape(sphere1);

    scene->addLight(l2);
    scene->addShape(sphere2);

    scene->addLight(l3);
    scene->addShape(sphere3);

    scene->addLight(l4);
    scene->addShape(sphere4);

    scene->addLight(l5);
    scene->addShape(sphere5);

    // render
#ifdef SAMPLER
    a3SamplerRenderer* renderer = new a3SamplerRenderer(256);
    renderer->enableGammaCorrection = true;
    renderer->enableToneMapping = false;
#endif

#ifdef SINGLE_RAY
    a3SingleRayRenderer* renderer = new a3SingleRayRenderer();
    renderer->singleRayX = 281;
    renderer->singleRayY = 527;
#endif
    renderer->sampler = new a3RandomSampler();
    renderer->camera = camera;

    a3DirectLighting* direcLighting = new a3DirectLighting(16, 16);
    renderer->integrator = direcLighting;
    //a3PathTracer* pathTracer = new a3PathTracer();
    //renderer->integrator = pathTracer;

    // rendering
    t3Timer timer;
    timer.start();

    renderer->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
}
