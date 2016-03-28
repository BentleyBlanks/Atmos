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

//#define WENDAOQIUER
#define INFINITEAREA_LIGHT
//#define CORNEL_BOX

int main()
{
    a3Film* image = new a3Film(2048, 1536, "../../../resources/results/hello", A3_IMAGE_PNG);
    a3NormalMap* normalMap = new a3NormalMap(*image);
    normalMap->setFileName("../../../resources/results/normalMap");
#ifdef INFINITEAREA_LIGHT
    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(0, 180, 40), t3Vector3f(0, 0, 25), t3Vector3f(0, 0, 1), 1.0f, 3.8, 3.8 * image->width / image->height, 1.0f, 210.0f, 0.0f, image, normalMap);
#endif

#ifdef WENDAOQIUER
    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(0, -150, -10), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 1.0f, 3.8, 3.8 * image->width / image->height, 1.0f, 210.0f, 0.0f, image, normalMap);
#endif

#ifdef CORNEL_BOX
    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(50, 52, 295.6), t3Vector3f(50, 52 - 0.042612, 295.6 - 1), t3Vector3f(0, 1, 0), 1.0f, 5.84, 5.84 * image->width / image->height, 1.0f, 210.0f, 0.0f, image, normalMap);
#endif
    camera->print();

    a3Scene* scene = new a3Scene();

    auto addShape = [&scene](a3Shape* s, t3Vector3f color, t3Vector3f emission, int type)
    {
        s->emission = emission;
        s->color = color;
        s->type = type;
        scene->addShape(s);
    };

#ifdef INFINITEAREA_LIGHT
    scene->addLight(new a3InfiniteAreaLight("../../../resources/images/skylightSurreal.png"));

    a3ModelImporter importer;
    std::vector<a3Shape*>* shapes = importer.load("../../../resources/models/test.obj");

    if(shapes)
    {
        for(auto s : *shapes)
            addShape(s, t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

        addShape(new a3Sphere(t3Vector3f(-40, -60, 25), 25), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
        addShape(new a3Sphere(t3Vector3f(40, 30, 25), 25), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

        // ÎÞÏÞÔ¶Æ½Ãæ
        addShape(new a3Plane(t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    }

#endif

#ifdef WENDAOQIUER
    scene->addLight(new a3InfiniteAreaLight("../../../resources/images/1.png"));

    addShape(new a3Sphere(t3Vector3f(-30, -8, 0), 37), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

    // 无限远平面
    addShape(new a3Plane(t3Vector3f(0, 0, -37), t3Vector3f(0, 0, 1)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
#endif

#ifdef CORNEL_BOX
    // Spheres  
    addShape(new a3Sphere(t3Vector3f(73, 16.5, 47), 16.5), t3Vector3f(.999, .999, .999), t3Vector3f(0, 0, 0), A3_MATERIAL_SPECULAR);
    addShape(new a3Sphere(t3Vector3f(27, 16.5, 78), 16.5), t3Vector3f(.999, .999, .999), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

    // Plane
    addShape(new a3Plane(t3Vector3f(1, 40.8, 81.6), t3Vector3f(1, 0, 0)), t3Vector3f(0.25, 0.25, 0.75), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(99, 40.8, 81.6), t3Vector3f(-1, 0, 0)), t3Vector3f(0.75, 0.25, 0.25), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(50, 40.8, 0), t3Vector3f(0, 0, 1)), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(50, 40.8, 310), t3Vector3f(0, 0, 1)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(50, 0, 81.6), t3Vector3f(0, 1, 0)), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(50, 81.6, 81.6), t3Vector3f(0, -1, 0)), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);

    // 光源 自发光
    addShape(new a3Sphere(t3Vector3f(50, 681.6 - 0.27, 81.6), 600), t3Vector3f(0, 0, 0), t3Vector3f(5000, 5000, 5000), A3_MATERIAL_DIFFUSS);
#endif

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