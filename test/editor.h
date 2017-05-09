#pragma once
#include <Atmos.h>
// Utils
t3Vector3f a3Float3ToVec3(float* f3)
{
    // 不做安全检查
    return t3Vector3f(f3[0], f3[1], f3[2]);
}

a3Spectrum a3Float3ToSpectrum(float* f3)
{
    // 不做安全检查
    return a3Spectrum(f3[0], f3[1], f3[2]);
}

a3Texture<a3Spectrum>* generateTexture(a3TextureData* texture)
{
    a3Texture<a3Spectrum>* t = NULL;

    switch(texture->type)
    {
    case A3_TEXTURE_IMAGE:
        t = new a3ImageTexture<a3Spectrum>(texture->imagePath.c_str());
        break;
    case A3_TEXTURE_CONSTANT:
        t = new a3ConstantTexture<a3Spectrum>(a3Float3ToSpectrum(texture->value));
        break;
    case A3_TEXTURE_CHECKBOARD:
        t = a3CreateChekerBoardTexture(texture->t1, texture->t2, texture->level);
        break;
    default:
        break;
    }

    return t;
}

a3BSDF* generateBSDF(a3MaterialData* material)
{
    a3BSDF* bsdf = NULL;

    switch(material->type)
    {
    case A3_MATERIAL_LAMBERTIAN:
        bsdf = new a3Diffuse(a3Float3ToSpectrum(material->R));
        break;
    case A3_MATERIAL_MIRROR:
        bsdf = new a3Conductor(a3Float3ToSpectrum(material->R));
        break;
    case A3_MATERIAL_GLASS:
        bsdf = new a3Dieletric(a3Float3ToSpectrum(material->R));
        break;
    default:
        break;
    }

    if(bsdf)
        bsdf->texture = generateTexture(&material->textureData);

    return bsdf;
}

a3Light* generateLight(a3LightData* light)
{
    a3Light* l = NULL;

    switch(light->type)
    {
    case A3_LIGHT_POINT:
    {
        l = new a3PointLight(a3Float3ToVec3(light->position), a3Float3ToSpectrum(light->emission));
        break;
    }
    case A3_LIGHT_AREA:
    {
        // --!not supported 
        l = new a3AreaLight(a3Float3ToSpectrum(light->emission));
        break;
    }
    case A3_LIGHT_INFINITE_AREA:
    {
        l = new a3InfiniteAreaLight(light->imagePath);
        break;
    }
    case A3_LIGHT_SPOT:
    {
        l = new a3SpotLight(a3Float3ToVec3(light->position),
                            a3Float3ToVec3(light->direction),
                            a3Float3ToSpectrum(light->emission),
                            light->coneAngle, light->falloffStart);
        break;
    }
    default:
        break;
    }

    return l;
}

a3Shape* generateShape(a3ShapeData* shape)
{
    a3Shape* s = NULL;

    switch(shape->type)
    {
    case A3_SHAPE_SPHERE:
    {
        s = new a3Sphere(a3Float3ToVec3(shape->position), shape->radius);
        break;
    }
    case A3_SHAPE_DISK:
    {
        s = new a3Disk(a3Float3ToVec3(shape->position), shape->radius, a3Float3ToVec3(shape->normal));
        break;
    }
    case A3_SHAPE_PLANE:
    {
        // --!not supported
        break;
    }
    case A3_SHAPE_TRIANGLE:
    {
        s = new a3Triangle(a3Float3ToVec3(shape->t1), a3Float3ToVec3(shape->t2), a3Float3ToVec3(shape->t3));
        break;
    }
    case A3_SHAPE_INFINITE_PLANE:
    {
        s = new a3InfinitePlane(a3Float3ToVec3(shape->position), a3Float3ToVec3(shape->normal));
        break;
    }
    default:
        break;
    }

    if(s)
    {
        // material
        a3BSDF* bsdf = generateBSDF(&shape->materialData);
        if(bsdf && bsdf->texture)
            s->bCalTextureCoordinate = true;
    }

    return s;
};

a3Scene* generateScene(a3S2CInitMessage* initMsg)
{
    a3Scene* scene = new a3Scene();

    // scene
    switch(initMsg->primitiveSetType)
    {
    case A3_PRIMITIVESET_EXHAUSTIVE:
        scene->primitiveSet = new a3Exhaustive();
        break;
    case A3_PRIMITIVESET_BVH:
        scene->primitiveSet = new a3BVH();
        break;
    }

    // shapes
    for(auto shape : initMsg->shapeList)
    {
        a3Shape* s = generateShape(&shape);
        if(s)
            scene->addShape(s);
    }

    // lights
    for(auto light : initMsg->lightList)
    {
        a3Light* l = generateLight(&light);
        if(l)
            scene->addLight(l);
    }

    if(initMsg->primitiveSetType == A3_PRIMITIVESET_BVH)
        ((a3BVH*) scene->primitiveSet)->init();

    return scene;
}

void main()
{
    a3IPCRenderer* renderer = new a3IPCRenderer();

    renderer->waiting();

    renderer->init();

    // ----------------------------------------allocate----------------------------------------
    a3S2CInitMessage* initMsg = renderer->getInitMessage();
    if(!initMsg)
    {
        a3Log::warning("Init Message not recieved");
        getchar();
        return;
    }

    // camera
    a3Film* film = new a3Film(initMsg->imageWidth, initMsg->imageHeight, initMsg->imagePath);

    if(initMsg->camera.type != "Perspective")
        a3Log::warning("Not support sensor type, already setted to perspective");

    a3Sensor* camera = new a3PerspectiveSensor(a3Float3ToVec3(initMsg->camera.origin),
                                     a3Float3ToVec3(initMsg->camera.lookat),
                                     a3Float3ToVec3(initMsg->camera.up),
                                     initMsg->camera.fov, initMsg->camera.focalDistance, initMsg->camera.lensRadius,
                                     film);
    camera->print();

    a3Scene* scene = generateScene(initMsg);

    // rendering
    t3Timer timer;
    timer.start();

    renderer->render(scene);

    timer.end();

    a3Log::info("Cost time: %f sec", timer.difference());

    getchar();

    // ----------------------------------------deallocate----------------------------------------
    // camera
    if(camera && camera->image)
    {
        A3_SAFE_DELETE(camera->image);
        A3_SAFE_DELETE(camera);
    }

    // scene
    if(scene)
    {
        for(auto l : scene->lights)
            A3_SAFE_DELETE(l);

        if(scene->primitiveSet)
        {
            for(auto s : scene->primitiveSet->primitives)
            {
                if(s && s->bsdf)
                {
                    A3_SAFE_DELETE(s->bsdf->texture);
                    A3_SAFE_DELETE(s->bsdf);
                }

                A3_SAFE_DELETE(s);
            }
        }

        A3_SAFE_DELETE(scene);
    }

    return;
}