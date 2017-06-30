#ifndef A3_MESSAGE_H
#define A3_MESSAGE_H

#include <core/messageQueue/a3MessageQueue.h>
#include <core/log/a3Log.h>

#define A3_MESSAGE_DEFINE(MessageClass, MessageType)      \
	MessageClass()  { type = MessageType; }            \

#define A3_FLOAT3CPY(f3Dest, f3Src)  f3Dest[0] = f3Src[0];\
                                     f3Dest[1] = f3Src[1];\
                                     f3Dest[2] = f3Src[2];

// grid size definition
#define A3_GRIDBUFFER_MAX_WIDTH 100
#define A3_GRIDBUFFER_MAX_HEIGHT 100
#define A3_GRIDBUFFER_LENGTH A3_GRIDBUFFER_MAX_WIDTH * A3_GRIDBUFFER_MAX_HEIGHT * 3

#define A3_ADDRESS_PATH_LENGTH 256

// Messages
enum a3C2SMessages
{
    A3_C2S_MSG_GRIDIMAGE = 0,
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
    A3_TEXTURE_NULL = 0,
    A3_TEXTURE_IMAGE,
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

enum a3CameraType
{
    A3_CAMERA_PERSPECTIVE = 0
};

enum a3ModelType
{
    A3_MODEL_OBJ = 0
};

// To String
inline std::string a3TypeToString(a3TextureType type);

inline std::string a3TypeToString(a3CameraType type);

inline std::string a3TypeToString(a3ShapeType type);

inline std::string a3TypeToString(a3MaterialType type);

inline std::string a3TypeToString(a3ModelType type);

inline std::string a3TypeToString(a3LightType type);

inline std::string a3TypeToString(a3IntegratorType type);

inline std::string a3TypeToString(a3PrimitiveSetType type);

// 全集
struct a3TextureData
{
    a3TextureType type = A3_TEXTURE_NULL;

    // image texture
    char imagePath[A3_ADDRESS_PATH_LENGTH] = "";

    // constant texture
    float value[3] = {0.0f, 0.0f, 0.0f};

    // checkboard texture
    float t1 = 0.2f, t2 = 0.8f, level = 22;

    inline a3TextureData& operator=(const a3TextureData& c);

    inline void print() const;
};

struct a3MaterialData
{
    a3MaterialType type = A3_MATERIAL_LAMBERTIAN;

    // all 材质反射率
    float R[3] = {1.0f, 1.0f, 1.0f};

    // texture
    a3TextureData textureData;

    inline a3MaterialData& operator=(const a3MaterialData& c);

    inline void print() const;
};

struct a3ShapeData
{
    a3ShapeType type = A3_SHAPE_SPHERE;

    // --!user defined material in the future
    a3MaterialData materialData;

    // sphere disk
    float radius = 0.0f;

    // triangle
    float t1[3] = {1.0f, 0.0f, 0.0f}, t2[3] = {0.0f, 1.0f, 0.0f}, t3[3] = {0.0f, 0.0f, 1.0f};

    // common
    float position[3] = {0.0f, 0.0f, 0.0f};

    // disk plane infinite_plane
    float normal[3] = {0.0f, 1.0f, 0.0f};

    // plane
    float width = 0.0f, height = 0.0f;

    inline a3ShapeData& operator=(const a3ShapeData& c);

    inline void print() const;
};

struct a3LightData
{
    a3LightType type = A3_LIGHT_POINT;

    // all 
    float emission[3] = {0.0f, 0.0f, 0.0f};

    // --------------------area--------------------
    a3ShapeData shape;

    // --------------------infinite light--------------------
    char imagePath[128];

    // --------------------point spot--------------------
    float position[3] = {0.0f, 0.0f, 0.0f};

    // --------------------spot--------------------
    float direction[3] = {0.0f, 0.0f, 0.0f};


    // 圆锥与中心线夹角(弧度)
    float coneAngle = 0.0f;

    // 开始产生半影的角度(弧度)
    float falloffStart = 0.0f;

    inline a3LightData& operator=(const a3LightData& c);

    inline void print() const;
};

struct a3ModelData
{
    // 默认仅支持.obj
    a3ModelType type;

    a3MaterialData materialData;

    char path[A3_ADDRESS_PATH_LENGTH] = "";

    inline a3ModelData& operator=(const const a3ModelData& c);

    inline void print() const;
};

struct a3CameraData
{
    a3CameraType type = A3_CAMERA_PERSPECTIVE;

    float origin[3] = {0.0f, 0.0f, 0.0f}, lookat[3] = {0.0f, 0.0f, 1.0f}, up[3] = {0.0f, 1.0f, 0.0f};

    float fov = 40.0f, focalDistance = 100.0f, lensRadius = 0.0f;

    inline a3CameraData& operator=(const a3CameraData& c);

    inline void print() const;
};

// Server to Client
// the info that rendered scene needed
struct a3S2CInitMessage : public a3MessageEntryHead
{
    inline a3S2CInitMessage(const a3S2CInitMessage& msg);

    inline void print() const;

    // camrea
    a3CameraData camera;

    // image
    // path可缺省只做编辑器预览
    char imagePath[A3_ADDRESS_PATH_LENGTH] = "";
    int imageWidth = A3_IMAGE_DEFAULT_WIDTH, imageHeight = A3_IMAGE_DEFAULT_HEIGHT;
    int levelX = A3_GRID_LEVELX, levelY = A3_GRID_LEVELY;

    // renderer
    //a3RendererType rendererType;
    a3IntegratorType integratorType = A3_INTEGRATOR_PATH;
    a3PrimitiveSetType primitiveSetType = A3_PRIMITIVESET_EXHAUSTIVE;

    // render
    int spp = 4;
    bool enableGammaCorrection = true, enableToneMapping = false;

    // path tracing
    int maxDepth = -1, russianRouletteDepth = 4;

    // shapes
    a3ShapeData shapeList[32];
    a3ModelData modelList[32];
    a3LightData lightList[32];

    int shapeListLength = 0, modelListLength = 0, lightListLength = 0;

    A3_MESSAGE_DEFINE(a3S2CInitMessage, A3_S2C_MSG_INIT)
};

// Client to Server
struct a3C2SGridBufferMessage : public a3MessageEntryHead
{
    int currentIndex;
    int gridX, gridY;
    int gridWidth, gridHeight;

    float origin[3], direction[3];

    // maximum length of buffer 
    float buffer[A3_GRIDBUFFER_LENGTH];

    A3_MESSAGE_DEFINE(a3C2SGridBufferMessage, A3_C2S_MSG_GRIDIMAGE)
};

struct a3C2SLightPathMessage : public a3MessageEntryHead
{
    float origin[3], direction[3];
    A3_MESSAGE_DEFINE(a3C2SLightPathMessage, A3_C2S_MSG_LIGHTPATH)
};

#include <core/messageQueue/a3Message.inl>

#endif