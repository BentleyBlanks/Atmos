#include <Atmos.h>

#define SAMPLER
//#define SINGLE_RAY

void main()
{
    // camera
    a3Film* image = new a3Film(1024, 1024, "../../../../resources/results/direct.png");

    a3PerspectiveSensor* camera = new a3PerspectiveSensor(t3Vector3f(0, 0, 50), 
                                                          t3Vector3f(0, 0, 1), 
                                                          t3Vector3f(0, 1, 0), 
                                                          40, 210.0f, 0.0f, image);

    // scene
    a3Scene* scene = new a3Scene();
    scene->primitiveSet = new a3Exhaustive();

    // objects
    t3Matrix4x4 diskToWorld;
    diskToWorld.set(1, 0, 0, 0,
                    0, 0,-1, 0,
                    0, 1, 0, 0,
                    0, 0, 0, 1);

    a3Disk* disk = new a3Disk(diskToWorld, diskToWorld.getInverse(), 5);
    a3Diffuse* diffuse = new a3Diffuse();
    disk->setBSDF(diffuse);
    //scene->addShape(disk);


    t3Matrix4x4 sphereToWorld;
    sphereToWorld.set(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
    a3Sphere* sphere = new a3Sphere(sphereToWorld, sphereToWorld.getInverse(), 5);
    sphere->setBSDF(diffuse);
    scene->addShape(sphere);

    // lights
    // environment
    t3Matrix4x4 emitterToWorld;
    emitterToWorld.set(1, 0, 0, 0,
                       0, 0, 1, 0,
                       0, 1, 0, 0,
                       0, 0, 0, 1);
    a3EnvironmentLight* ibl = new a3EnvironmentLight("../../../../resources/images/9.png", emitterToWorld);
    scene->addLight(ibl);

    // render
#ifdef SAMPLER
    a3SamplerRenderer* renderer = new a3SamplerRenderer(1024);
    renderer->enableGammaCorrection = false;
    renderer->enableToneMapping = false;
#endif

#ifdef SINGLE_RAY
    a3SingleRayRenderer* renderer = new a3SingleRayRenderer();
    renderer->singleRayX = 512;
    renderer->singleRayY = 512;
#endif
    renderer->sampler = new a3RandomSampler();
    renderer->camera = camera;
    renderer->integrator = new a3DirectLighting();

    // rendering
    t3Timer timer;
    timer.start();

    renderer->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
}
