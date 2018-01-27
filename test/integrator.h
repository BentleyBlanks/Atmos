#include <Atmos.h>

// Renderer
#define SAMPLER
//#define SINGLE_RAY

// Light
//#define ENVIRONMENT_LIGHT
#define AREA_LIGHT

void main()
{
    // camera
    a3Film* image = new a3Film(768, 768, "../../../../resources/results/bb.png");

    a3PerspectiveSensor* camera = new a3PerspectiveSensor(t3Vector3f(0, -3, 18), 
                                                          t3Vector3f(0, -3, 0), 
                                                          t3Vector3f(0, 1, 0), 
                                                          40, 210.0f, 0.0f, image);

    // scene
    a3Scene* scene = new a3Scene();
    scene->primitiveSet = new a3Exhaustive();

#ifdef ENVIRONMENT_LIGHT
    // objects
    t3Matrix4x4 diskToWorld;
    diskToWorld.set(1, 0, 0, 0,
                    0, 0,-1, 0,
                    0, 1, 0, 0,
                    0, 0, 0, 1);
    a3Disk* disk = new a3Disk(diskToWorld, 5);
    a3Diffuse* diffuse = new a3Diffuse();
    disk->setBSDF(diffuse);
    //scene->addShape(disk);


    t3Matrix4x4 sphereToWorld;
    sphereToWorld.set(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
    a3Sphere* sphere = new a3Sphere(sphereToWorld, 5);
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
#endif
#ifdef AREA_LIGHT
    // Matrix
    t3Matrix4x4 bottomM, topM, rightM, leftM, forwardM, areaM;
    bottomM.set(1, 0, 0, 0,
                0, 1, 0, -10,
                0, 0, 1, 0,
                0, 0, 0, 1);

    topM.set(1, 0, 0, 0,
             0, -1, 0, 2.01,
             0, 0, -1, 0,
             0, 0, 0, 1);

    rightM.set(0, 1, 0, -7,
              -1, 0, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1);

    leftM.set(0, -1, 0, 7,
              1, 0, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1);

    forwardM.set(1, 0, 0, 0,
                 0, 0, -1, 0,
                 0, 1, 0, -17,
                 0, 0, 0, 1);

    areaM.set(1, 0, 0, 0,
              0, -1, 0, 2,
              0, 0, -1, 0,
              0, 0, 0, 1);

    t3Matrix4x4 sphereM1, sphereM2;
    sphereM1.set(1, 0, 0, 2.5,
                 0, 1, 0, -7.5,
                 0, 0, 1, -10,
                 0, 0, 0, 1);

    sphereM2.set(1, 0, 0, -2.5,
                 0, 1, 0, -8.5,
                 0, 0, 1, -5,
                 0, 0, 0, 1);
    // wall
    a3Disk* top = new a3Disk(topM, 40);
    a3Disk* bottom = new a3Disk(bottomM, 40);
    a3Disk* left = new a3Disk(leftM, 40);
    a3Disk* right = new a3Disk(rightM, 40);
    a3Disk* forward = new a3Disk(forwardM, 40);

    // spheres
    a3Sphere* sphere1 = new a3Sphere(sphereM1, 2.5);
    a3Sphere* sphere2 = new a3Sphere(sphereM2, 1.5);

    // light
    a3Disk* light = new a3Disk(areaM, 1.5);
    a3AreaLight* areaLight = new a3AreaLight(light, a3Spectrum(10), areaM);

    // material
    a3Diffuse* topD = new a3Diffuse();
    a3Diffuse* bottomD = new a3Diffuse();
    a3Diffuse* leftD = new a3Diffuse(a3Spectrum(1.0, 0.2, 0.2));
    a3Diffuse* rightD = new a3Diffuse(a3Spectrum(0.2, 1.0, 0.2));
    a3Diffuse* forwardD = new a3Diffuse();

    a3Diffuse* sphereD1 = new a3Diffuse();
    a3Diffuse* sphereD2 = new a3Diffuse();

    a3Conductor* conductor1 = new a3Conductor();
    a3Conductor* conductor2 = new a3Conductor();

    top->setBSDF(topD);
    bottom->setBSDF(bottomD);
    left->setBSDF(leftD);
    right->setBSDF(rightD);
    forward->setBSDF(forwardD);

    //sphere1->setBSDF(sphereD1);
    //sphere2->setBSDF(sphereD2);
    sphere1->setBSDF(conductor1);
    sphere2->setBSDF(conductor2);

    // scene
    scene->addLight(areaLight);
    scene->addShape(light);

    scene->addShape(top);
    scene->addShape(bottom);
    scene->addShape(left);
    scene->addShape(right);
    scene->addShape(forward);

    scene->addShape(sphere1);
    scene->addShape(sphere2);
#endif

    // render
#ifdef SAMPLER
    a3SamplerRenderer* renderer = new a3SamplerRenderer(32);
    renderer->enableGammaCorrection = true;
    renderer->enableToneMapping = false;
#endif

#ifdef SINGLE_RAY
    a3SingleRayRenderer* renderer = new a3SingleRayRenderer();
    renderer->singleRayX = 296;
    renderer->singleRayY = 522;
#endif
    renderer->sampler = new a3RandomSampler();
    renderer->camera = camera;
    
    //a3DirectLighting* direcLighting = new a3DirectLighting(16, 0);
    //renderer->integrator = direcLighting;
    a3PathTracer* pathTracer = new a3PathTracer();
    renderer->integrator = pathTracer;

    // rendering
    t3Timer timer;
    timer.start();

    renderer->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
}
