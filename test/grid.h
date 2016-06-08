#include <Atmos.h>

enum a3MaterialType
{
    NONE = 0,
    LAMBERTIAN = 1,
    MIRROR = 2,
    GLASS = 3
};

int main()
{
    // alloc
    a3Film* image = new a3Film(1024, 768, "../../../../resources/results/grid.png");

    a3PerspectiveSensor* camera = camera = new a3PerspectiveSensor(t3Vector3f(0, 80, 20), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 40, 100.0f, 0.0f, image);

    a3PathIntegrator* path = new a3PathIntegrator();
    path->russianRouletteDepth = 3;
    path->maxDepth = -1;

    a3BVH* bvh = NULL;
    a3Scene* scene = new a3Scene();
    scene->primitiveSet = bvh = new a3BVH();

    a3GridRenderer* r = new a3GridRenderer(8);
    r->setLevel(5, 5);
    r->camera = camera;
    r->sampler = new a3RandomSampler();
    r->integrator = path;
    r->enableGammaCorrection = false;
    r->enableToneMapping = true;
    r->startX = 0;
    r->startY = 0;
    r->renderWidth = 700;
    r->renderHeight = 200;

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

    scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/grace-new_latlong.exr"));

    a3ModelImporter importer;
    std::vector<a3Shape*>* blender = importer.load("../../../../resources/models/blender.obj");

    for(auto s : *blender)
        addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), GLASS, NULL);

    bvh->init();

    // rendering
    t3Timer timer;
    timer.start();

    r->begin();

    for(int i = 0; i < r->levelX * r->levelY; i++)
        r->render(scene);

    r->end();

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
    return 0;
}
