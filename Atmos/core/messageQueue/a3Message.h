#pragma once
#include <core/messageQueue/a3MessageQueue.h>

enum A3_C2S_MESSAGES
{
    A3_C2S_MSG_GRIDIMAGE = 0,
    //A3_C2S_MSG_FINISHED,
    // --!in the future
    A3_C2S_MSG_LIGHTPATH
};

enum A3_S2C_MESSAGES
{
    A3_S2C_MSG_INIT = 5,
};

#define A3_MESSAGE_DEFINE(MessageClass, MessageType)      \
	MessageClass()  { type = MessageType; }            \

// grid size definition
#define A3_GRIDBUFFER_MAX_WIDTH 100
#define A3_GRIDBUFFER_MAX_HEIGHT 100
#define A3_GRIDBUFFER_LENGTH A3_GRIDBUFFER_MAX_WIDTH * A3_GRIDBUFFER_MAX_HEIGHT * 3

// 全集
struct a3LightSet
{
    std::string type;

    // all 
    float emission[3];

    // --------------------area--------------------
    a3ShapeSet shape;

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

struct a3ShapeSet
{
    std::string type;

    // sphere disk
    float radius;

    // common
    float position[3];

    // disk plane infinite_plane
    float normal[3];

    // plane
    float width, height;
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
    float origin[3], lookAt[3], up[3];
    float fov, focalDistance, lensRadius;

    // image
    int imageWidth, imageHeight;
    int levelX, levelY;
    
    // renderer
    std::string rendererName;

    // render
    int spp;    
    bool enableGammaCorrection;
    bool enableToneMapping;

    // scene
    std::string acceleratorName;

    // shapes


    // models
    std::vector<std::string> modelList;

    // lights

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