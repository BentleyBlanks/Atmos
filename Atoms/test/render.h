#include <renderers/a3SamplerRenderer.h>
#include <cameras/a3PerspectiveCamera.h>
#include <core/image/a3Image.h>
#include <core/a3Scene.h>
#include <shapes/a3Sphere.h>
#include <core/log/a3Log.h>
#include <shapes/a3Plane.h>
#include <shapes/a3Disk.h>

int main()
{
    a3Image* image = new a3Image(1920, 1920, "hello", A3_IMAGE_PNG);

    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(0, 0, -120), t3Vector3f(0, 0, 1), 1.0f, 3.89332f, 3.89332f * image->width / image->height, 1.0f, image);
    a3Log::debug("fov: %f, %f\n", t3Math::Rad2Deg(camera->fov.x), t3Math::Rad2Deg(camera->fov.y));

    a3Scene* scene = new a3Scene();

    auto addShape = [&scene](a3Shape* s, t3Vector3f color, t3Vector3f emission, int type)
    {
        s->emission = emission;
        s->color = color;
        s->type = type;
        scene->objects.push_back(s);
    };


    // Sphere/Disk    
    addShape(new a3Sphere(t3Vector3f(-30, -74, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);
    addShape(new a3Sphere(t3Vector3f(0, 0, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);
    addShape(new a3Disk(t3Vector3f(30, 74, 100), 15, t3Vector3f(0, 0, 1)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);
    addShape(new a3Sphere(t3Vector3f(-30, 74, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);
    addShape(new a3Sphere(t3Vector3f(30, -59, 100), 30), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), A3_METERIAL_REFRACTION);

    // Plane
    addShape(new a3Plane(t3Vector3f(-70, 0, 100), t3Vector3f(-1, 0, 0)), t3Vector3f(7, 2, 2), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(70, 0, 100), t3Vector3f(1, 0, 0)), t3Vector3f(2, 2, 7), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(0, 0, 140), t3Vector3f(0, 0, 1)), t3Vector3f(7, 7, 9), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(0, 90, 0), t3Vector3f(0, 1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);
    addShape(new a3Plane(t3Vector3f(0, -90, 0), t3Vector3f(0, -1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), A3_MATERIAL_DIFFUSS);

    // 光源 自发光
    addShape(new a3Disk(t3Vector3f(-69, 0, 100), 100, t3Vector3f(-1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(140, 40, 40), A3_MATERIAL_DIFFUSS);
    addShape(new a3Disk(t3Vector3f(69, 0, 100), 100, t3Vector3f(1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(40, 140, 40), A3_MATERIAL_DIFFUSS);

    a3SamplerRenderer* renderer = new a3SamplerRenderer();
    renderer->camera = camera;

    renderer->render(scene);

    //getchar();
    return 0;
}