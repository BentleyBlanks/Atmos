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
#include <core/image/a3NormalMap.h>

int main()
{
    a3Film* image = new a3Film(2000, 2000, "../../../resources/results/hello", A3_IMAGE_PNG);
    a3NormalMap* normalMap = new a3NormalMap(*image);
    normalMap->setFileName("../../../resources/results/normalMap");

    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(0, 80, -120), t3Vector3f(0, 0, 0), t3Vector3f(0, 1, 0), 1.0f, 3.8, 3.8 * image->width / image->height, 1.0f, 210.0f, 0.0f, image, normalMap);

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

    scene->addLight(new a3InfiniteAreaLight("../../../resources/images/skylightPollution.png"));

    a3ModelImporter importer;
    std::vector<a3Shape*>* shapes = importer.load("../../../resources/models/test.obj");

    if(shapes)
    {
        for(auto s : *shapes)
            addShape(s, t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

        addShape(new a3Sphere(t3Vector3f(-30, 20, 30), 20), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
        addShape(new a3Sphere(t3Vector3f(30, 20, -30), 20), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

        // 无限远平面
        addShape(new a3Plane(t3Vector3f(0, 0, 0), t3Vector3f(0, 1, 0)), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    }

    //// Sphere/Disk    
	//addShape(new a3Sphere(t3Vector3f(-0, -0, 0), 35), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    //addShape(new a3Sphere(t3Vector3f(0, 0, 90), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
	//addShape(new a3Disk(t3Vector3f(0, 0, -35), 60, t3Vector3f(0, 0, -1)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
	//addShape(new a3Sphere(t3Vector3f(-30, 74, 70), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);
    //addShape(new a3Sphere(t3Vector3f(30, -59, 60), 30), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    
    //// Plane
    //addShape(new a3Plane(t3Vector3f(-48, 0, 81.6), t3Vector3f(1, 0, 0)), t3Vector3f(1.5, 1, 1), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    //addShape(new a3Plane(t3Vector3f(48, 0, 81.6), t3Vector3f(-1, 0, 0)), t3Vector3f(1, 1, 1.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    //addShape(new a3Plane(t3Vector3f(0, 0, 170), t3Vector3f(0, 0, -1)), t3Vector3f(1.5, 1.5, 1.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    //addShape(new a3Plane(t3Vector3f(0, 40.8, 0), t3Vector3f(0, -1, 0)), t3Vector3f(1.5, 1.5, 1.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    //addShape(new a3Plane(t3Vector3f(0, -40.8, 0), t3Vector3f(0, 1, 0)), t3Vector3f(1.5, 1.5, 1.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);

    //// 光源 自发光
    //addShape(new a3Sphere(t3Vector3f(0, 640.8 - 0.27, 125), 600), t3Vector3f(0, 0, 0), t3Vector3f(1000, 1000, 1000), A3_MATERIAL_DIFFUSS);
    //addShape(new a3Disk(t3Vector3f(0, 89.995, 100), 30, t3Vector3f(0, -1, 0)), t3Vector3f(0, 0, 0), t3Vector3f(10000, 10000, 10000), A3_MATERIAL_DIFFUSS);
    //addShape(new a3Disk(t3Vector3f(69, 0, 100), 50, t3Vector3f(1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(10000, 10000, 10000), A3_MATERIAL_DIFFUSS);


    a3SamplerRenderer* renderer = new a3SamplerRenderer();
    renderer->camera = camera;

    t3Timer timer;
    timer.start();

    renderer->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
    return 0;
}