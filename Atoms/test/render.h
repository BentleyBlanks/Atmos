#include <renderers/a3SamplerRenderer.h>
#include <cameras/a3PerspectiveCamera.h>
#include <core/image/a3Film.h>
#include <core/a3Scene.h>
#include <shapes/a3Sphere.h>
#include <core/log/a3Log.h>
#include <shapes/a3Plane.h>
#include <shapes/a3Disk.h>
#include <shapes/a3Triangle.h>
#include <Common/t3Timer.h>
#include <core/a3ModelImporter.h>
#include <lights/a3InfiniteAreaLight.h>
#include <core/a3Common.h>

//#define SCENE_1
#define SCENE_2

#define FRAME_ANIMATION

int main()
{
    a3Film* image = new a3Film(900, 900, "hello", A3_IMAGE_PNG);

    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(-100, 0, 0), t3Vector3f(0, 0, 0), t3Vector3f(0, 1, 0), 1.0f, 2, 2 * image->width / image->height, 1.0f, 210.0f, 0.0f, image);

    a3Log::debug("fov: %f, %f\n", t3Math::Rad2Deg(camera->fov.x), t3Math::Rad2Deg(camera->fov.y));
    a3Log::debug("focal distance: %f, lens radius: %f\n", camera->focalDistance, camera->lensRadius);

    a3Scene* scene = new a3Scene();

    auto addShape = [&scene](a3Shape* s, t3Vector3f color, t3Vector3f emission, int type)
    {
        s->emission = emission;
        s->color = color;
        s->type = type;
        scene->addShape(s);
    };

    //scene->addLight(new a3InfiniteAreaLight("10.png"));

#ifdef SCENE_1
    a3ModelImporter importer;
    std::vector<a3Shape*>* shapes = importer.load("./test.obj");

    if(shapes)
    {
        for(auto s : *shapes)
            addShape(s, t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    }

    // Sphere/Disk    
    addShape(new a3Sphere(t3Vector3f(-30, -74, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    addShape(new a3Sphere(t3Vector3f(0, 0, 90), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    addShape(new a3Disk(t3Vector3f(30, 74, 80), 15, t3Vector3f(0, 0, 1)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Triangle(t3Vector3f(25, 0, 100), t3Vector3f(25, 90, 80), t3Vector3f(45, 90, 90)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    addShape(new a3Sphere(t3Vector3f(-30, 74, 70), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    addShape(new a3Sphere(t3Vector3f(30, -59, 60), 30), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    addShape(new a3Triangle(t3Vector3f(0.8058, -7.3278, -19.7270), t3Vector3f(19.7270, -7.3278, -0.8058), t3Vector3f(-0.8058, -7.3278, 19.7270)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);

    // Plane
    addShape(new a3Plane(t3Vector3f(-70, 0, 100), t3Vector3f(-1, 0, 0)), t3Vector3f(7, 2, 2), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(70, 0, 100), t3Vector3f(1, 0, 0)), t3Vector3f(2, 2, 7), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(0, 0, 140), t3Vector3f(0, 0, 1)), t3Vector3f(7, 7, 9), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(0, 90, 0), t3Vector3f(0, 1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(0, -90, 0), t3Vector3f(0, -1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);

    // 光源 自发光
    addShape(new a3Disk(t3Vector3f(-69, 0, 100), 100, t3Vector3f(-1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(160, 40, 40), A3_MATERIAL_DIFFUSS);
    addShape(new a3Disk(t3Vector3f(69, 0, 100), 100, t3Vector3f(1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(40, 160, 40), A3_MATERIAL_DIFFUSS);
    addShape(new a3Disk(t3Vector3f(-69, 0, 100), 100, t3Vector3f(-1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(140, 40, 40), A3_MATERIAL_DIFFUSS);

#endif

#ifdef SCENE_2
    addShape(new a3Sphere(t3Vector3f(0, 0, 0), 35), t3Vector3f(4, 8, 4), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Disk(t3Vector3f(0, -35, 0), 70, t3Vector3f(0, -1, 0)), t3Vector3f(0, 0, 0), t3Vector3f(140, 40, 40), A3_MATERIAL_SPECULAR);
    //addShape(new a3Plane(t3Vector3f(0, -35, 0), t3Vector3f(0, -1, 0)), t3Vector3f(7, 2, 2), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
#endif


    a3SamplerRenderer* renderer = new a3SamplerRenderer();
    renderer->camera = camera;

    t3Timer timer;
    timer.start();

#ifdef FRAME_ANIMATION
    for(int i = -300; i < 300; i+=9)
    {
        camera->setCameraToWorld(t3Vector3f(i, 0, -100), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1));

        image->setFileName("frames/" + a3ToString(i));
#endif
        renderer->render(scene);

#ifdef FRAME_ANIMATION
    }
#endif


    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
    return 0;
}