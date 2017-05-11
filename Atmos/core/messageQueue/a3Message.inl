#ifndef A3_MESSAGE_INL
#define A3_MESSAGE_INL

#include <core/messageQueue/a3Message.h>

// To String
std::string a3TypeToString(a3TextureType type)
{
    switch(type)
    {
    case A3_TEXTURE_NULL:
        return "Null";
    case A3_TEXTURE_IMAGE:
        return "Image Texture";
    case A3_TEXTURE_CONSTANT:
        return "Constant Texture";
    case A3_TEXTURE_CHECKBOARD:
        return "CheckBoard Texture";
    default:
        return "Unknown Type Texture";
    }
}

std::string a3TypeToString(a3CameraType type)
{
    switch(type)
    {
    case A3_CAMERA_PERSPECTIVE:
        return "Perspective Camera";
    default:
        return "Unknown Type Camera";
    }
}

std::string a3TypeToString(a3ShapeType type)
{
    switch(type)
    {
    case A3_SHAPE_SPHERE:
        return "Sphere";
    case A3_SHAPE_DISK:
        return "Disk";
    case A3_SHAPE_PLANE:
        return "Plane";
    case A3_SHAPE_TRIANGLE:
        return "Triangle";
    case A3_SHAPE_INFINITE_PLANE:
        return "Infinite Plane";
    default:
        return "Unknown Type Shape";
    }
}


std::string a3TypeToString(a3MaterialType type)
{
    switch(type)
    {
    case A3_MATERIAL_LAMBERTIAN:
        return "Lambertian";
    case A3_MATERIAL_MIRROR:
        return "Mirror";
    case A3_MATERIAL_GLASS:
        return "Glass";
    default:
        return "Unknown Type Material";
    }
}

std::string a3TypeToString(a3ModelType type)
{
    switch(type)
    {
    case A3_MODEL_OBJ:
        return "Obj Model";
    default:
        return "Unknown Type Model";
    }
}

std::string a3TypeToString(a3LightType type)
{
    switch(type)
    {
    case A3_LIGHT_POINT:
        return "Point Light";
    case A3_LIGHT_AREA:
        return "Area Light";
    case A3_LIGHT_INFINITE_AREA:
        return "Infinite Area Light";
    case A3_LIGHT_SPOT:
        return "Spot Light";
    default:
        return "Unknown Type Light";
    }
}

std::string a3TypeToString(a3IntegratorType type)
{
    switch(type)
    {
    case A3_INTEGRATOR_PATH:
        return "Path Tracing";
    case A3_INTEGRATOR_DIRECT_LIGHTING:
        return "Direct Lighting";
    default:
        return "Unknown Type";
    }
}

std::string a3TypeToString(a3PrimitiveSetType type)
{
    switch(type)
    {
    case A3_PRIMITIVESET_EXHAUSTIVE:
        return "Exhaustive";
    case A3_PRIMITIVESET_BVH:
        return "BVH";
    default:
        return "Unknown Type";
    }
}

// Data Set
a3TextureData& a3TextureData::operator=(const a3TextureData& c)
{
    type = c.type;

    strcpy(imagePath, c.imagePath);

    A3_FLOAT3CPY(value, c.value);

    t1 = c.t1;
    t2 = c.t2;
    level = c.level;
    return *this;
}

void a3TextureData::print() const
{
    a3Log::debug("Texture type:%s\n", a3TypeToString(type).c_str());

    switch(type)
    {
    case A3_TEXTURE_IMAGE:
        a3Log::debug("Image Texture imagePath:%s\n", imagePath);
        break;
    case A3_TEXTURE_CONSTANT:
        a3Log::debug("Constant Texture value[%f, %f, %f]\n", value[0], value[1], value[2]);
        break;
    case A3_TEXTURE_CHECKBOARD:
        a3Log::debug("CheckBoard Texture t1:%f, t2:%f, level:%f\n", t1, t2, level);
        break;
    case A3_TEXTURE_NULL:
    default:
        break;
    }
}

a3MaterialData& a3MaterialData::operator=(const a3MaterialData& c)
{
    type = c.type;
    A3_FLOAT3CPY(R, c.R);
    textureData = c.textureData;

    return *this;
}

void a3MaterialData::print() const
{
    a3Log::debug("Material type:%s\n", a3TypeToString(type).c_str());
    a3Log::debug("Material R[%f, %f, %f]\n", R[0], R[1], R[2]);

    if(textureData.type != A3_TEXTURE_NULL)
        textureData.print();
}

a3ShapeData& a3ShapeData::operator=(const a3ShapeData& c)
{
    type = c.type;
    materialData = c.materialData;

    radius = c.radius;

    A3_FLOAT3CPY(t1, c.t1);
    A3_FLOAT3CPY(t2, c.t2);
    A3_FLOAT3CPY(t3, c.t3);

    A3_FLOAT3CPY(position, c.position);
    A3_FLOAT3CPY(normal, c.normal);

    width = c.width;
    height = c.height;
    return *this;
}

void a3ShapeData::print() const
{
    a3Log::debug("Shape type:%s\n", a3TypeToString(type).c_str());
    materialData.print();

    switch(type)
    {
    case A3_SHAPE_SPHERE:
        a3Log::debug("Sphere position[%f, %f, %f]\n", position[0], position[1], position[2]);
        a3Log::debug("Sphere radius:%f\n", radius);
        break;
    case A3_SHAPE_DISK:
        a3Log::debug("Disk position[%f, %f, %f], normal[%f, %f, %f]\n",
                     position[0], position[1], position[2],
                     normal[0], normal[1], normal[2]);
        a3Log::debug("Disk radius:%f\n", radius);
        break;
    case A3_SHAPE_PLANE:
        a3Log::debug("Plane dimension[%f, %f]\n", width, height);
        a3Log::debug("Plane position[%f, %f, %f], normal[%f, %f, %f]\n",
                     position[0], position[1], position[2],
                     normal[0], normal[1], normal[2]);
        break;
    case A3_SHAPE_TRIANGLE:
        a3Log::debug("Triangle t1[%f, %f, %f], t2[%f, %f, %f], t3[%f, %f, %f]\n",
                     t1[0], t1[1], t1[2],
                     t2[0], t2[1], t2[2],
                     t3[0], t3[1], t3[2]);
        break;
    case A3_SHAPE_INFINITE_PLANE:
        a3Log::debug("Infinite Plane position[%f, %f, %f], normal[%f, %f, %f]\n",
                     position[0], position[1], position[2],
                     normal[0], normal[1], normal[2]);
        break;
    }
}

a3LightData& a3LightData::operator=(const a3LightData& c)
{
    type = c.type;
    A3_FLOAT3CPY(emission, c.emission);
    A3_FLOAT3CPY(position, c.position);
    A3_FLOAT3CPY(direction, c.direction);

    // operator =
    shape = c.shape;

    strcpy(imagePath, c.imagePath);

    coneAngle = c.coneAngle;
    falloffStart = c.falloffStart;
    return *this;
}

void a3LightData::print() const
{
    a3Log::debug("Light type:%s\n", a3TypeToString(type).c_str());

    switch(type)
    {
    case A3_LIGHT_POINT:
        a3Log::debug("Point Light position[%f, %f, %f]\n", position[0], position[1], position[2]);
        a3Log::debug("Point Light intensity[%f, %f, %f]\n", emission[0], emission[1], emission[2]);
        break;
    case A3_LIGHT_AREA:
        a3Log::debug("Area Light intensity[%f, %f, %f]\n");
        shape.print();
        break;
    case A3_LIGHT_INFINITE_AREA:
        a3Log::debug("InfiniteArea Light imagePath:%s\n", imagePath);
        break;
    case A3_LIGHT_SPOT:
        a3Log::debug("Spot Light position[%f, %f, %f], direction[%f, %f, %f]\n",
                     position[0], position[1], position[2],
                     direction[0], direction[1], direction[2]);
        a3Log::debug("Spot Light intensity[%f, %f, %f]\n", emission[0], emission[1], emission[2]);
        a3Log::debug("Spot Light coneAngle:%f, falloffStart:%f\n", coneAngle, falloffStart);
        break;
    }
}


a3ModelData& a3ModelData::operator=(const a3ModelData& c)
{
    type = c.type;
    strcpy(path, c.path);
    materialData = c.materialData;
    return *this;
}

void a3ModelData::print() const
{
    a3Log::debug("Model type:%s\n", a3TypeToString(type).c_str());
    a3Log::debug("Model path:%s\n", path);
    materialData.print();
}

a3CameraData& a3CameraData::operator=(const a3CameraData& c)
{
    type = c.type;
    A3_FLOAT3CPY(origin, c.origin);
    A3_FLOAT3CPY(lookat, c.lookat);
    A3_FLOAT3CPY(up, c.up);

    fov = c.fov;
    focalDistance = c.focalDistance;
    lensRadius = c.lensRadius;
    return *this;
}

void a3CameraData::print() const
{
    a3Log::debug("Camera type:%s\n", a3TypeToString(type).c_str());
    a3Log::debug("Camera origin[%f, %f, %f] lookat[%f, %f, %f] up[%f, %f, %f]\n",
                 origin[0], origin[1], origin[2],
                 lookat[0], lookat[1], lookat[2],
                 up[0], up[1], up[2]);
    a3Log::debug("Camera fov:%f focalDistance:%f lensRadius:%f\n", fov, focalDistance, lensRadius);
}

a3S2CInitMessage::a3S2CInitMessage(const a3S2CInitMessage& msg) :
    imageWidth(msg.imageWidth), imageHeight(msg.imageHeight),
    levelX(msg.levelX), levelY(msg.levelY),
    spp(msg.spp), enableGammaCorrection(msg.enableGammaCorrection), enableToneMapping(msg.enableToneMapping),
    maxDepth(msg.maxDepth), russianRouletteDepth(msg.russianRouletteDepth),
    integratorType(msg.integratorType), primitiveSetType(msg.primitiveSetType),
    shapeListLength(msg.shapeListLength), modelListLength(msg.modelListLength), lightListLength(msg.lightListLength)
{
    // operator =
    camera = msg.camera;

    // shapes
    for(int i = 0; i < msg.shapeListLength; i++)
        shapeList[i] = msg.shapeList[i];

    // lights
    for(int i = 0; i < msg.lightListLength; i++)
        lightList[i] = msg.lightList[i];

    // models
    for(int i = 0; i < msg.modelListLength; i++)
        modelList[i] = msg.modelList[i];

    strcpy(imagePath, msg.imagePath);
}

void a3S2CInitMessage::print() const
{
    a3Log::info("------------------------------a3S2CInitMessage Info Begin------------------------------\n");
    camera.print();
    a3Log::debug("Film path:%s, dimension[%d, %d], level[%d, %d]\n", imagePath, imageWidth, imageHeight, levelX, levelY);
    a3Log::debug("Integrator type:%s, maxDepth:%d, russianRouletteDepth:%d\n", a3TypeToString(integratorType).c_str(), maxDepth, russianRouletteDepth);
    a3Log::debug("PrimitiveSet type:%s\n", a3TypeToString(primitiveSetType).c_str());
    a3Log::debug("Renderer spp:%d, enableGammaCorrection:%d, enableToneMapping:%d\n", spp, enableGammaCorrection, enableToneMapping);

    for(int i = 0; i < shapeListLength; i++)
    {
        a3Log::debug("------------------------Shape Index[%d / %d] Begin\n------------------------", i, shapeListLength);
        shapeList[i].print();
        a3Log::debug("------------------------Shape Index[%d / %d] End------------------------\n", i, shapeListLength);
    }

    for(int i = 0; i < modelListLength; i++)
    {
        a3Log::debug("------------------------Model Index[%d / %d] Begin------------------------\n", i, modelListLength);
        modelList[i].print();
        a3Log::debug("------------------------Model Index[%d / %d] End------------------------\n", i, modelListLength);
    }

    for(int i = 0; i < lightListLength; i++)
    {
        a3Log::debug("------------------------Light Index[%d / %d] Begin------------------------\n", i, lightListLength);
        lightList[i].print();
        a3Log::debug("------------------------Light Index[%d / %d] End------------------------\n", i, lightListLength);
    }

    a3Log::info("------------------------------a3S2CInitMessage Info End------------------------------\n");
}

#endif