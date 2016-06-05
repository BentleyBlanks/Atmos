#include <Atmos.h>

enum a3MaterialType
{
    NONE = 0,
    LAMBERTIAN = 1,
    MIRROR = 2,
    GLASS = 3
};

//#define SINGLERAY

int main()
{
    // alloc
    a3Film* image = new a3Film(1024, 768, "../../../../resources/results/fresnel.png");

    a3PerspectiveSensor* camera = new a3PerspectiveSensor(t3Vector3f(50.0f, 52.0f, 295.6f), t3Vector3f(50.0f, 52.0f - 0.042612f, 295.6f - 1.0f), t3Vector3f(0, 1, 0), 40.0f, 210.0f, 0.0f, image);
    
    a3SamplerRenderer* r = new a3SamplerRenderer();
    r->camera = camera;
    r->sampler = new a3RandomSampler();

#ifdef SINGLERAY
    a3SingleRayRenderer* r = new a3SingleRayRenderer();
    r->camera = camera;
    r->sampler = new a3RandomSampler();
    r->singleRayX = 500;
    r->singleRayY = 350;
#else
    r->enableGammaCorrection = false;
    r->enableToneMapping = false;
    r->spp = 8;
#endif

    a3PathIntegrator* path = new a3PathIntegrator();
    path->russianRouletteDepth = 3;
    path->maxDepth = -1;
    r->integrator = path;

    a3Scene* scene = new a3Scene();
    scene->primitiveSet = new a3Exhaustive();

    a3AreaLight* areaLight = new a3AreaLight(a3Spectrum(10000));

    auto addShape = [&scene](a3Shape* s, a3Spectrum R, a3Spectrum emission, int type, a3Texture<a3Spectrum>* texture)->auto
    {
        s->emission = emission;

        switch(type)
        {
        case LAMBERTIAN:
            s->bsdf = new a3Diffuse(R);
            break;
        case MIRROR:
            s->bsdf = new a3Conductor(R);
            break;
        case GLASS:
            s->bsdf = new a3Dieletric(R);
            break;
        default:
            break;
        }

        s->bsdf->texture = texture;
        if(texture)
            s->bCalTextureCoordinate = true;

        scene->addShape(s);

        return s->bsdf;
    };

    // Spheres  
    addShape(new a3Sphere(t3Vector3f(73.0f, 16.5f, 47.0f), 16.5f), t3Vector3f(1.0f), t3Vector3f(0.0f, 0.0f, 0.0f), MIRROR, NULL);
    addShape(new a3Sphere(t3Vector3f(27.0f, 16.5f, 78.0f), 16.5f), t3Vector3f(1.0f), t3Vector3f(0.0f, 0.0f, 0.0f), GLASS, NULL);

    //// Plane
    addShape(new a3InfinitePlane(t3Vector3f(1.0f, 40.8f, 81.6f), t3Vector3f(1.0f, 0.0f, 0.0f), 500, 500), t3Vector3f(0.25f, 0.25f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    addShape(new a3InfinitePlane(t3Vector3f(99.0f, 40.8f, 81.6f), t3Vector3f(-1.0f, 0.0f, 0.0f), 500, 500), t3Vector3f(0.75f, 0.25f, 0.25f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    addShape(new a3InfinitePlane(t3Vector3f(50.0f, 40.8f, 0.0f), t3Vector3f(0.0f, 0.0f, 1.0f), 500, 500), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    //addShape(new a3Plane(t3Vector3f(50.0f, 40.8f, 310.0f), t3Vector3f(0.0f, 0.0f, 1.0f), 500, 500), t3Vector3f(0.0f, 0.0f, 0.0f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    addShape(new a3InfinitePlane(t3Vector3f(50.0f, 0.0f, 81.6f), t3Vector3f(0.0f, 1.0f, 0.0f), 50, 50), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    addShape(new a3InfinitePlane(t3Vector3f(50.0f, 81.6f, 81.6f), t3Vector3f(0.0f, -1.0f, 0.0f), 100, 100), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    //addShape(new a3Disk(t3Vector3f(50.0f, 81.6f, 81.6f), 500, t3Vector3f(0, -1.0f, 0)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);

    areaLight->addShape(new a3Disk(t3Vector3f(50.0f, 81.6f - 0.27f, 81.6f), 30, t3Vector3f(0.0f, -1.0f, 0.0f)));

    // ¹âÔ´
    //scene->addLight(areaLight);
    scene->addLight(new a3PointLight(t3Vector3f(50.0f, 81.6f - 0.27f, 81.6f), a3Spectrum(1000.0f)));

    // rendering
    t3Timer timer;
    timer.start();

    r->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
    return 0;
}