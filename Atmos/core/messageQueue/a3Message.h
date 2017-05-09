#pragma once
#include <core/messageQueue/a3MessageQueue.h>

#define A3_MESSAGE_DEFINE(MessageClass, MessageType)      \
	MessageClass()  { type = MessageType; }            \

// grid size definition
#define A3_GRIDBUFFER_MAX_WIDTH 100
#define A3_GRIDBUFFER_MAX_HEIGHT 100
#define A3_GRIDBUFFER_LENGTH A3_GRIDBUFFER_MAX_WIDTH * A3_GRIDBUFFER_MAX_HEIGHT * 3

// Messages
enum a3C2SMessages
{
    A3_C2S_MSG_GRIDIMAGE = 0,
    //A3_C2S_MSG_FINISHED,
    // --!in the future
    A3_C2S_MSG_LIGHTPATH
};

enum a3S2CMessages
{
    A3_S2C_MSG_INIT = 5,
};

// Atmos Types
enum a3ShapeType
{
    A3_SHAPE_SPHERE = 0,
    A3_SHAPE_DISK,
    A3_SHAPE_PLANE,
    A3_SHAPE_TRIANGLE,
    A3_SHAPE_INFINITE_PLANE,
};

enum a3LightType
{
    A3_LIGHT_POINT = 0,
    A3_LIGHT_AREA,
    A3_LIGHT_INFINITE_AREA,
    A3_LIGHT_SPOT,
};

enum a3MaterialType
{
    A3_MATERIAL_LAMBERTIAN = 0,
    A3_MATERIAL_MIRROR,
    A3_MATERIAL_GLASS
};

enum a3TextureType
{
    A3_TEXTURE_IMAGE = 0,
    A3_TEXTURE_CONSTANT,
    A3_TEXTURE_CHECKBOARD
};

enum a3RendererType
{
    A3_RENDERER_SINGLERAY = 0,
    A3_RENDERER_NORMALMAP,
    A3_RENDERER_SAMPLER
};

enum a3IntegratorType
{
    A3_INTEGRATOR_PATH = 0,
    A3_INTEGRATOR_DIRECT_LIGHTING
};

enum a3PrimitiveSetType
{
    A3_PRIMITIVESET_EXHAUSTIVE = 0,
    A3_PRIMITIVESET_BVH
};

// 全集
struct a3TextureData
{
    a3TextureType type;

    // image texture
    std::string imagePath;

    // constant texture
    float value[3];

    // checkboard texture
    float t1, t2, level;
};

struct a3MaterialData
{
    a3MaterialType type;

    // all 材质反射率
    float R[3];

    // texture
    a3TextureData textureData;
};

struct a3ShapeData
{
    a3ShapeType type;

    // --!user defined material in the future
    a3MaterialData materialData;

    // sphere disk
    float radius;

    // triangle
    float t1[3], t2[3], t3[3];

    // common
    float position[3];

    // disk plane infinite_plane
    float normal[3];

    // plane
    float width, height;
};

struct a3LightData
{
    a3LightType type;

    // all 
    float emission[3];

    // --------------------area--------------------
    a3ShapeData shape;

    // --------------------infinite light--------------------
    std::string imagePath;

    // --------------------point spot--------------------
    float position[3];

    // --------------------spot--------------------
    float direction[3];


    // 圆锥与中心线夹角(弧度)
    float coneAngle;

    // 开始产生半影的角度(弧度)
    float falloffStart;
    float coneHeight;
};

struct a3ModelData
{
    // 默认仅支持.obj
    std::string type;

    std::string path;
};

struct a3CameraData
{
    std::string type;

    float origin[3], lookat[3], up[3];

    float fov, focalDistance, lensRadius;
};

// Server to Client
// the info that rendered scene needed
struct a3S2CInitMessage : public a3MessageEntryHead
{
    a3S2CInitMessage(const a3S2CInitMessage& msg) :
        imageWidth(msg.imageWidth), imageHeight(msg.imageHeight),
        levelX(msg.levelX), levelY(msg.levelY),
        spp(msg.spp), enableGammaCorrection(msg.enableGammaCorrection), enableToneMapping(msg.enableToneMapping)
    {}

    // camrea
    a3CameraData camera;

    // image
    // path可缺省只做编辑器预览
    std::string imagePath;
    int imageWidth, imageHeight;
    int levelX, levelY;
    
    // renderer
    //a3RendererType rendererType;
    a3IntegratorType integratorType;
    a3PrimitiveSetType primitiveSetType;

    // render
    int spp;    
    bool enableGammaCorrection;
    bool enableToneMapping;
    // path tracing
    int maxDepth, russianRouletteDepth;

    // shapes
    std::vector<a3ShapeData> shapeList;

    // models
    std::vector<a3ModelData> modelList;

    // lights
    std::vector<a3LightData> lightList;

    A3_MESSAGE_DEFINE(a3S2CInitMessage, A3_S2C_MSG_INIT)
};

// Client to Server
struct a3C2SGridBufferMessage : public a3MessageEntryHead
{
    int currentIndex;
    int gridX, gridY;
    int gridWidth, gridHeight;

    // maximum length of buffer 
    float buffer[A3_GRIDBUFFER_LENGTH];

    A3_MESSAGE_DEFINE(a3C2SGridBufferMessage, A3_C2S_MSG_GRIDIMAGE)
};

//struct a3C2SFinishedMessage : public a3MessageEntryHead
//{
//    A3_MESSAGE_DEFINE(a3C2SFinishedMessage, A3_C2S_MSG_FINISHED)
//};