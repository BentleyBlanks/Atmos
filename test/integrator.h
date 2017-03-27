#include <Atmos.h>

enum a3MaterialType
{
    NONE = 0,
    LAMBERTIAN = 1,
    MIRROR = 2,
    GLASS = 3
};

enum a3SceneName
{
    WENDAOQIUER = 0,
    INFINITEAREA_LIGHT = 1,
    CORNEL_BOX = 2,
    BVHTEST = 3,
    WALLPAPER = 4
};

enum a3RendererName
{
    SINGLERAY = 0,
    NORMALMAP = 1,
    SAMPLER = 2
};

enum a3IntegratorName
{
    PATH = 0,
    DIRECT_LIGHTING = 1
};

enum a3PrimitiveSetName
{
    EXHAUSTIVE = 0,
    BVH = 1
};

// global config
int singleX = 350, singleY = 350;
int spp = 1;
int maxDepth = -1;
int russianRouletteDepth = 3;
int imageWidth = 200, imageHeight = 200;
bool enableGammaCorrection = true;
bool enableToneMapping = false;

a3SceneName name = WALLPAPER;
a3RendererName rendererName = SAMPLER;
a3IntegratorName integratorName = PATH;
a3PrimitiveSetName primitiveName = BVH;

inline a3PerspectiveSensor* generateCamera(a3Film* image, a3SceneName name)
{
    a3PerspectiveSensor* camera = NULL;

    if(name == INFINITEAREA_LIGHT)
        camera = new a3PerspectiveSensor(t3Vector3f(0, 50, -100), t3Vector3f(0, 0, 20), t3Vector3f(0, 1, 0), 40, 210.0f, 0.0f, image);
    else if(name == WENDAOQIUER)
        camera = new a3PerspectiveSensor(t3Vector3f(0, -150, -10), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 55.517f, 210.0f, 0.0f, image);
    else if(name == CORNEL_BOX)
        camera = new a3PerspectiveSensor(t3Vector3f(50.0f, 52.0f, 295.6f), t3Vector3f(50.0f, 52.0f - 0.042612f, 295.6f - 1.0f), t3Vector3f(0, 1, 0), 40.0f, 210.0f, 0.0f, image);
    else if(name == BVHTEST)
        camera = new a3PerspectiveSensor(t3Vector3f(0, 100, 50), t3Vector3f(0, -20, 0), t3Vector3f(0, 0, 1), 40, 100.0f, 0.0f, image);
    else if (name == WALLPAPER)
        camera = new a3PerspectiveSensor(t3Vector3f(0, 1500, 30), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 40, 1500.0f, 35, image);

    camera->print();

    return camera;
}

inline a3Renderer* gengerateRenderer(a3PerspectiveSensor* camera, a3Film* image, a3RendererName name, a3IntegratorName integratorName)
{
    a3Renderer* renderer = NULL;

    if(name == SINGLERAY)
    {
        a3SingleRayRenderer* r = new a3SingleRayRenderer(singleX, singleY);

        r->sampler = new a3RandomSampler();
        r->camera = camera;

        renderer = r;
    }
    else if(name == NORMALMAP)
    {
        a3NormalMapRenderer* r = new a3NormalMapRenderer();

        r->normalMap = new a3NormalMap(*image);
        r->normalMap->setFileName("../../../../resources/results/normalMap.png");
        r->sampler = new a3RandomSampler();
        r->camera = camera;

        renderer = r;
    }
    else if(name == SAMPLER)
    {
        a3SamplerRenderer* r = new a3SamplerRenderer(spp);

        r->enableGammaCorrection = enableGammaCorrection;
        r->enableToneMapping = enableToneMapping;
        r->sampler = new a3RandomSampler();
        r->camera = camera;

        renderer = r;
    }

    if(name == SINGLERAY || name == SAMPLER)
    {
        a3SamplerRenderer* r = (a3SamplerRenderer*)renderer;

        if(integratorName == PATH)
            r->integrator = new a3PathIntegrator(russianRouletteDepth, maxDepth);
        else if(integratorName == DIRECT_LIGHTING)
            r->integrator = new a3DirectLightingIntegrator();
    }
    
    return renderer;
}

inline a3Scene* generateScene(a3SceneName name, a3PrimitiveSetName primitiveName)
{
    a3BVH* bvh = NULL;
    a3Scene* scene = new a3Scene();

    if(primitiveName == EXHAUSTIVE)
        scene->primitiveSet = new a3Exhaustive();
    else if(primitiveName == BVH)
        scene->primitiveSet = bvh = new a3BVH();

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

    if(name == INFINITEAREA_LIGHT)
    {
        //scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/skylightSurreal.png"));
        scene->addLight(new a3PointLight(t3Vector3f(0, 10, -100), a3Spectrum(500000.0f)));

        a3ModelImporter importer;
        //std::vector<a3Shape*> shapes = importer.load("../../../../resources/models/spheres.obj");
        std::vector<a3Shape*> shapes = importer.load("../../../../resources/models/spheres.obj");

        if(shapes.size() > 0)
        {
            for(auto s : shapes)
                addShape(s, t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), GLASS, NULL);

            //addShape(new a3Sphere(t3Vector3f(-40, -60, 25), 25), t3Vector3f(1.0f, 1.0f, 1.0f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
            //addShape(new a3Sphere(t3Vector3f(40, 30, 25), 25), t3Vector3f(1.0f, 1.0f, 1.0f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);

            //addShape(new a3Plane(t3Vector3f(0, 0, -20), t3Vector3f(0, 0, 1)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
            addShape(new a3Disk(t3Vector3f(0, 0, 0), 1000, t3Vector3f(0, 1, 0)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
        }
    }
    else if(name == WENDAOQIUER)
    {
        scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/envmap.exr"));

       // a3ImageTexture<a3Spectrum>* texture = a3CreateImageTexture("../../../../resources/images/earth.png"),
         //   *texture2 = a3CreateImageTexture("../../../../resources/images/wood3.png");
        //a3CheckerBoard<a3Spectrum>* texture2 = a3CreateChekerBoardTexture();

        addShape(new a3Sphere(t3Vector3f(-30, -8, 0), 37), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);

        // 无限远平面
        //addShape(new a3Disk(t3Vector3f(0, 0, -37), 5000, t3Vector3f(0, 0, 1)), t3Vector3f(0.5, 0.5, 0.5), t3Vector3f(0, 0, 0), LAMBERTIAN, texture2);
        addShape(new a3InfinitePlane(t3Vector3f(0, 0, -37), t3Vector3f(0, 0, 1)), t3Vector3f(0.5f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    }
    else if(name == CORNEL_BOX)
    {
        // Spheres  
        addShape(new a3Sphere(t3Vector3f(73.0f, 16.5f, 47.0f), 16.5f), t3Vector3f(1.0f), t3Vector3f(0.0f, 0.0f, 0.0f), LAMBERTIAN, NULL);
        addShape(new a3Sphere(t3Vector3f(27.0f, 16.5f, 78.0f), 16.5f), t3Vector3f(1.0f), t3Vector3f(0.0f, 0.0f, 0.0f), GLASS, NULL);

        // Plane
        addShape(new a3InfinitePlane(t3Vector3f(1.0f, 40.8f, 81.6f), t3Vector3f(1.0f, 0.0f, 0.0f)), t3Vector3f(0.25f, 0.25f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
        addShape(new a3InfinitePlane(t3Vector3f(99.0f, 40.8f, 81.6f), t3Vector3f(-1.0f, 0.0f, 0.0f)), t3Vector3f(0.75f, 0.25f, 0.25f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
        addShape(new a3InfinitePlane(t3Vector3f(50.0f, 40.8f, 0.0f), t3Vector3f(0.0f, 0.0f, 1.0f)), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
        //addShape(new a3Plane(t3Vector3f(50.0f, 40.8f, 310.0f), t3Vector3f(0.0f, 0.0f, 1)), t3Vector3f(0.0f, 0.0f, 0.0f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
        addShape(new a3InfinitePlane(t3Vector3f(50.0f, 0.0f, 81.6f), t3Vector3f(0.0f, 1.0f, 0.0f)), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
        addShape(new a3InfinitePlane(t3Vector3f(50.0f, 81.6f, 81.6f), t3Vector3f(0.0f, -1.0f, 0.0f)), t3Vector3f(0.75f, 0.75f, 0.75f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);

        // 光源
        //addShape(new a3Sphere(t3Vector3f(50.0f, 681.6f - 0.27f, 81.6f), 600.0f), t3Vector3f(0.0f, 0.0f, 0.0f), t3Vector3f(5000.0f, 5000.0f, 5000.0f), LAMBERTIAN, NULL);
        scene->addLight(new a3PointLight(t3Vector3f(50.0f, 81.6f - 0.27f, 81.6f), a3Spectrum(3000.0f)));
        //scene->addLight(new a3SpotLight(t3Vector3f(50.0f, 81.6f - 0.27f, 81.6f), t3Vector3f(0.0f, -1.0f, 0.0f), a3Spectrum(3000000.0f), 40, 5));
    }
    else if(name == BVHTEST)
    {
        //scene->addLight(new a3PointLight(t3Vector3f(0.0f, 100.0f, 10.0f), a3Spectrum(10000.0f)));
        scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/0.png"));
        //scene->addLight(new a3PointLight(t3Vector3f(0, 80.0f, 80.0f), a3Spectrum(500000.0f)));

        a3ModelImporter importer;
        //std::vector<a3Shape*>* plane = importer.load("../../../../resources/models/mitsuba/mitsuba_plane.obj");
        //std::vector<a3Shape*>* internal = importer.load("../../../../resources/models/mitsuba/mitsuba_internal.obj");
        //std::vector<a3Shape*>* sphere = importer.load("../../../../resources/models/mitsuba/mitsuba_sphere.obj");
        std::vector<a3Shape*> mitsuba = importer.load("../../../../resources/models/mitsuba.obj");

        a3BSDF* bsdf = NULL;
        a3CheckerBoard<a3Spectrum>* texture = a3CreateChekerBoardTexture();
        //a3ImageTexture<a3Spectrum>* texture = a3CreateImageTexture("../../../../resources/images/earth.png");

        if(mitsuba.size() > 0)
            for(auto s : mitsuba)
                bsdf = addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), MIRROR, NULL);

        //// plane
        //if(plane)
        //    for(auto s : *plane)
        //        bsdf = addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), LAMBERTIAN, texture);

        //// internal
        //if(internal)
        //    for(auto s : *internal)
        //        bsdf = addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);

        //// sphere
        //if(sphere)
        //    for(auto s : *sphere)
        //        bsdf = addShape(s, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), MIRROR, NULL);

        //std::vector<a3Shape*>* triangle = importer.load("../../../../resources/models/sphere.obj");
        //if(triangle)
        //    for(auto t : *triangle)
        //        bsdf = addShape(t, t3Vector3f(1.0f), t3Vector3f(0, 0, 0), GLASS, NULL);

        //t3Vector3f normal(1, 0, 0), nx(0, 0, 1), ny(0, 1, 0), p(0, 0, 0);
        //t3Matrix4x4 worldToPlane;
        //worldToPlane._mat[0].set(ny.x, nx.x, normal.x, 0.f);
        //worldToPlane._mat[1].set(ny.y, nx.y, normal.y, 0.f);
        //worldToPlane._mat[2].set(ny.z, nx.z, normal.z, 0.f);
        //worldToPlane._mat[3].set(0.f, 0.f, 0.f, 1.f);

        //t3Matrix4x4 localToWolrd;
        //localToWolrd._mat[0].set(nx.x, nx.y, nx.z, 0.f);
        //localToWolrd._mat[1].set(ny.x, ny.y, ny.z, 0.f);
        //localToWolrd._mat[2].set(normal.x, normal.y, normal.z, 0.f);
        //localToWolrd._mat[3].set(0.f, 0.f, 0.f, 1.f);

        //a3Plane* plane = new a3Plane(worldToPlane, p, 50, 50);
        //addShape(plane, t3Vector3f(1.0f), t3Vector3f(0.0f), LAMBERTIAN, NULL);

        // 地球仪
        addShape(new a3Sphere(t3Vector3f(0.f), 30), a3Spectrum(1.f), a3Spectrum(0.f), LAMBERTIAN, texture);
    }
    else if(name == WALLPAPER)
    {
        scene->addLight(new a3InfiniteAreaLight("../../../../resources/images/skylightBlue.png"));

        a3ModelImporter importer;
        std::vector<a3Shape*> triangle = importer.load("../../../../resources/models/sibenik.obj");

        if(triangle.size() > 0)
        {
            a3Log::debug("add shape\n");
            for(auto s : triangle)
                addShape(s, t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);

            a3Log::debug("primitive number:%d\n", bvh->primitives.size());
        }

        //a3CheckerBoard<a3Spectrum>* texture = a3CreateChekerBoardTexture();


        //addShape(new a3Sphere(t3Vector3f(80, 0, 20), 70), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), GLASS, NULL);

        //addShape(new a3Sphere(t3Vector3f(-80, 0, 10), 60), t3Vector3f(0.8, 0.8, 0.8), t3Vector3f(0, 0, 0), LAMBERTIAN, texture);

        //addShape(new a3Sphere(t3Vector3f(-150, 500, 10), 60), t3Vector3f(1, 1, 1), t3Vector3f(0, 0, 0), MIRROR, NULL);

        //addShape(new a3Sphere(t3Vector3f(320, -800, 40), 90), t3Vector3f(0.2, 0.9, 0.2), t3Vector3f(0, 0, 0), LAMBERTIAN, texture);

        //addShape(new a3InfinitePlane(t3Vector3f(0, 0, -50), t3Vector3f(0.0f, 0.0f, 1.0f)), t3Vector3f(0.35f, 0.35f, 0.35f), t3Vector3f(0, 0, 0), LAMBERTIAN, NULL);
    }

    // 加速结构初始化
    if(primitiveName == BVH)
        bvh->init();

    return scene;
}

int main()
{
    // alloc
    a3Film* image = new a3Film(imageWidth, imageHeight, "../../../../resources/results/hi.png");

    a3PerspectiveSensor* camera = generateCamera(image, name);

    a3Scene* scene = generateScene(name, primitiveName);

    a3Renderer* renderer = gengerateRenderer(camera, image, rendererName, integratorName);

    // rendering
    t3Timer timer;
    timer.start();

    renderer->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();
    return 0;
} 