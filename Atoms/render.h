#include <renderers/a3SamplerRenderer.h>
#include <cameras/a3PerspectiveCamera.h>
#include <image/a3Image.h>
#include <core/a3Scene.h>
#include <shapes/a3Sphere.h>
#include <core/log/a3Log.h>

int main()
{
    a3Image* image = new a3Image(900, 900, "hello.ppm");

    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 1.0f, 2.0f, 2.0f, 1.0f, image);
    a3Log::debug("fov: %f, %f\n", t3Math::Rad2Deg(camera->fov.x), t3Math::Rad2Deg(camera->fov.y));

    a3Scene* scene = new a3Scene();
    a3Sphere* sphere = new a3Sphere(t3Vector3f(0, 0, 5), 1);
    scene->objects.push_back(sphere);
    
    a3SamplerRenderer* renderer = new a3SamplerRenderer();
    renderer->camera = camera;

    renderer->render(scene);

    getchar();
    return 0;
}