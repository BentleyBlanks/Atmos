#include <renderers/a3SamplerRenderer.h>
#include <cameras/a3PerspectiveCamera.h>
#include <image/a3Image.h>
#include <core/a3Scene.h>
#include <shapes/a3Sphere.h>
#include <core/log/a3Log.h>

int main()
{
    a3Image* image = new a3Image(900, 900, "hello.ppm");

    a3PerspectiveCamera* camera = new a3PerspectiveCamera(t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 1.0f, 2.0f, 2.0f * image->width / image->height, 1.0f, image);
    a3Log::debug("fov: %f, %f\n", t3Math::Rad2Deg(camera->fov.x), t3Math::Rad2Deg(camera->fov.y));

    a3Scene* scene = new a3Scene();

    auto addSphere = [&scene](a3Shape* s, t3Vector3f color, t3Vector3f emission, int type)
    {
        s->emission = emission;
        s->color = color;
        scene->objects.push_back(s);
    };

    // 极大的圆近似做Plane
    //addSphere(new a3Sphere(t3Vector3f(1e5+1, 40.8, 81.6), 1e5), t3Vector3f(0.75, 0.25, 0.25), t3Vector3f(0, 0, 0), 0);
    //addSphere(new a3Sphere(t3Vector3f(-1e5+99, 40.8, 81.6), 1e5), t3Vector3f(0.25, 0.25, 0.75), t3Vector3f(0, 0, 0), 0);
    //addSphere(new a3Sphere(t3Vector3f(50, 40.8, 1e5), 1e5), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), 0);
    //addSphere(new a3Sphere(t3Vector3f(50, 40.8, -1e5+170), 1e5), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), 0);
    //addSphere(new a3Sphere(t3Vector3f(50, 1e5, 81.6), 1e5), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), 0);
    //addSphere(new a3Sphere(t3Vector3f(50, -1e5 + 81.6, 81.6), 1e5), t3Vector3f(0.75, 0.75, 0.75), t3Vector3f(0, 0, 0), 0);

    // 实体球
    addSphere(new a3Sphere(t3Vector3f(0, 0, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), 0);
    addSphere(new a3Sphere(t3Vector3f(-20, 0, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), 0);
    addSphere(new a3Sphere(t3Vector3f(20, 0, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), 0);

    // 光源 自发光
    addSphere(new a3Sphere(t3Vector3f(0, 200, 100), 150), t3Vector3f(0, 0, 0), t3Vector3f(100, 100, 100), 0);
    //addSphere(new a3Sphere(t3Vector3f(0, -160, 100), 80), t3Vector3f(0, 0, 0), t3Vector3f(50, 50, 50), 0);

    a3SamplerRenderer* renderer = new a3SamplerRenderer();
    renderer->camera = camera;

    renderer->render(scene);

    //getchar();
    return 0;
}