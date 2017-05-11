#include "a3IPCRenderer.h"

// core
#include <core/a3Warp.h>
#include <core/log/a3Log.h>
#include <core/a3Scene.h>
#include <core/a3Ray.h>

#include <core/image/a3Film.h>
#include <sensors/a3PerspectiveSensor.h>

#include <samples/a3RandomSampler.h>
#include <samples/a3CameraSample.h>

//integrator
#include <integrator/a3PathIntegrator.h>
#include <integrator/a3DirectLighting.h>

inline t3Vector3f a3Float3ToVec3(float* f3)
{
    // 不做安全检查
    return t3Vector3f(f3[0], f3[1], f3[2]);
}

inline a3Spectrum a3Float3ToSpectrum(float* f3)
{
    // 不做安全检查
    return a3Spectrum(f3[0], f3[1], f3[2]);
}

// a3IPCRenderer
a3IPCRenderer::a3IPCRenderer()
    : spp(128),
    levelX(A3_GRID_LEVELX), levelY(A3_GRID_LEVELY),
    sampler(NULL), camera(NULL), integrator(NULL), gridBuffer(NULL),
    enableGammaCorrection(true), enableToneMapping(false), finished(false),
    imageWidth(0), imageHeight(0), gridWidth(0), gridHeight(0),
    /*currentGrid(0),*/ gridCount(0), initMsg(NULL)
{
}

a3IPCRenderer::~a3IPCRenderer()
{
    // 释放二维buffer指针
    for(int i = 0; i < gridCount; i++)
        A3_SAFE_DELETE_1DARRAY(gridBuffer[i]);
    A3_SAFE_DELETE_1DARRAY(gridBuffer);

    // --!未来更替为智能指针
    A3_SAFE_DELETE(sampler);

    // camera
    if(camera && camera->image)
    {
        A3_SAFE_DELETE(camera->image);
        A3_SAFE_DELETE(camera);
    }

    // integrator
    A3_SAFE_DELETE(integrator);

    A3_SAFE_DELETE(initMsg);
}

//void a3IPCRenderer::setLevel(int levelX, int levelY)
//{
//    if(levelX > 0)
//        this->levelX = levelX;
//    else
//        this->levelX = A3_GRID_LEVELX;
//
//    if(levelY > 0)
//        this->levelY = levelY;
//    else
//        this->levelX = A3_GRID_LEVELY;
//}

//void a3IPCRenderer::setCurrentGrid(int currentGrid)
//{
//    if(currentGrid > 0 && currentGrid <= gridCount)
//        this->currentGrid = currentGrid;
//    else
//        this->currentGrid = 0;
//}

void a3IPCRenderer::waiting()
{
    const int msgMaxNum = 100;
    const int S2CMsgSize = sizeof(a3S2CInitMessage);
    const int C2SMsgSize = sizeof(a3C2SGridBufferMessage);

    ipcS2C.init(L"Who's Your Daddy S2C", false, msgMaxNum / 2, S2CMsgSize);
    ipcC2S.init(L"Who's Your Daddy C2S", false, msgMaxNum, C2SMsgSize);

    // ------------------------------------------------Waiting------------------------------------------------
    // waiting till initMsg. 60fps
    bool recieved = false;
    while(!recieved)
    {
        if(!ipcS2C.isEmpty())
        {
            // stack maybe full
            char* msg_buffer = new char[S2CMsgSize];
            memset(msg_buffer, 0, S2CMsgSize);

            a3MessageEntryHead* pMsg = (a3MessageEntryHead*) msg_buffer;
            while(ipcS2C.dequeue(*pMsg))
            {
                if(pMsg->type == A3_S2C_MSG_INIT)
                {
                    // 接收初始化消息
                    const a3S2CInitMessage* msg = (const a3S2CInitMessage*) pMsg;
                    initMsg = new a3S2CInitMessage(*msg);
                    initMsg->print();
                    recieved = true;
                }
            }
            delete msg_buffer;
        }
        // 限帧
        Sleep(16.66667);
    }
}

void a3IPCRenderer::init()
{
    if(initMsg)
    {
        // init ipc renderer with specific configs
        initRenderer();

        // allocate the image
        initBuffer();
    }
    else
        a3Log::error("Not recieved init message from server");
}

//void a3IPCRenderer::initScene(a3S2CInitMessage* initMsg)
//{
//    // camera
//    a3Film* film = new a3Film(initMsg->imageWidth, initMsg->imageHeight, initMsg->imagePath);
//
//    if(initMsg->camera.type != "Perspective")
//        a3Log::warning("Not support sensor type, already setted to perspective");
//
//    camera = new a3PerspectiveSensor(a3Float3ToVec3(initMsg->camera.origin),
//                                     a3Float3ToVec3(initMsg->camera.lookat),
//                                     a3Float3ToVec3(initMsg->camera.up),
//                                     initMsg->camera.fov, initMsg->camera.focalDistance, initMsg->camera.lensRadius,
//                                     film);
//    camera->print();
//
//    // scene
//    switch(initMsg->primitiveSetType)
//    {
//    case A3_PRIMITIVESET_EXHAUSTIVE:
//        scene->primitiveSet = new a3Exhaustive();
//        break;
//    case A3_PRIMITIVESET_BVH:
//        scene->primitiveSet = new a3BVH();
//        break;
//    }
//
//    // shapes
//    for(auto shape : initMsg->shapeList)
//    {
//        a3Shape* s = generateShape(&shape);
//        if(s) 
//            scene->addShape(s);
//    }
//
//    // lights
//    for(auto light : initMsg->lightList)
//    {
//        a3Light* l = generateLight(&light);
//        if(l)
//            scene->addLight(l);
//    }
//
//    if(initMsg->primitiveSetType == A3_PRIMITIVESET_BVH)
//        ((a3BVH*)scene->primitiveSet)->init();
//}

void a3IPCRenderer::initRenderer()
{
    // camera
    a3Film* film = new a3Film(initMsg->imageWidth, initMsg->imageHeight, initMsg->imagePath);

    if(initMsg->camera.type != A3_CAMERA_PERSPECTIVE)
        a3Log::warning("Not support sensor type, already setted to perspective\n");

    camera = new a3PerspectiveSensor(a3Float3ToVec3(initMsg->camera.origin),
                                     a3Float3ToVec3(initMsg->camera.lookat),
                                     a3Float3ToVec3(initMsg->camera.up),
                                     initMsg->camera.fov, initMsg->camera.focalDistance, initMsg->camera.lensRadius,
                                     film);

    a3Log::print("\n");
    a3Log::info("------------------------------a3IPCRenderer Camera Begin------------------------------\n");
    camera->print();
    a3Log::info("------------------------------a3IPCRenderer Camera End------------------------------\n");

    enableGammaCorrection = initMsg->enableGammaCorrection;
    enableToneMapping = initMsg->enableToneMapping;
    spp = initMsg->spp;
    sampler = new a3RandomSampler();

    switch(initMsg->integratorType)
    {
    case A3_INTEGRATOR_PATH:
        integrator = new a3PathIntegrator(initMsg->russianRouletteDepth, initMsg->maxDepth);
        break;
    case A3_INTEGRATOR_DIRECT_LIGHTING:
        integrator = new a3DirectLightingIntegrator();
        break;
    default:
        break;
    }
}

void a3IPCRenderer::initBuffer()
{
    imageWidth = initMsg->imageWidth <= 0 ? A3_IMAGE_DEFAULT_WIDTH : initMsg->imageWidth;
    imageHeight = initMsg->imageHeight <= 0 ? A3_IMAGE_DEFAULT_HEIGHT : initMsg->imageHeight;

    levelX = initMsg->levelX <= 0 ? A3_GRID_LEVELX : initMsg->levelX;
    levelY = initMsg->levelY <= 0 ? A3_GRID_LEVELY : initMsg->levelY;
    gridCount = levelX * levelY;

    // 初始化网格渲染信息
    gridWidth = imageWidth / levelX;
    if(imageWidth % levelX != 0)
        a3Log::warning("a3IPCRenderer 网格在x轴上无法完整划分，渲染结果中可能出现黑边\n");

    gridHeight = imageHeight / levelY;
    if(imageHeight % levelY != 0)
        a3Log::warning("a3IPCRenderer 网格在y轴上无法完整划分，渲染结果中可能出现黑边\n");

    // 单一网格中像素数量
    int gridSize = gridWidth * gridHeight * 3;

    // grid buffer pointer
    gridBuffer = new float*[gridCount];
    for(int i = 0; i < gridCount; i++)
        gridBuffer[i] = new float[gridSize];
}

void a3IPCRenderer::render(const a3Scene* scene)
{        
    // ------------------------------------------------Check------------------------------------------------
    if(!check())
    {
        a3Log::warning("IPC Grid Rendering Start Failed\n");
        return;
    }
    else
        a3Log::success("IPC Grid Rendering Started\n");

    // 多块并行多核利用率优于单块多线程
#pragma omp parallel for schedule(dynamic)
    for(int currentGrid = 0; currentGrid < gridCount; currentGrid++)
    {
        // ------------------------------------------------Render------------------------------------------------
        // pointer to the current grid buffer
        float* bufferPointer = gridBuffer[currentGrid];

        // 局部Grid渲染
        //int gridX = (int) (currentGrid % levelX) * gridWidth;
        //int gridY = (int) (currentGrid / levelX) * gridHeight;

        //int gridEndX = gridX + gridWidth;
        //int gridEndY = gridY + gridHeight;

        a3Log::info("Rendering Grid[%d / %d] Spp:%d\n", currentGrid, gridCount, spp);
        //#pragma omp parallel for schedule(dynamic)
        //for(int x = gridX; x < gridEndX; x++)
        for(int x = 0; x < gridWidth; x++)
        {
            //progress = (float) (x - gridX) / gridWidth;
            //a3Log::info("Rendering: %8.2f \r",  progress * 100);
            //for(int y = gridY; y < gridEndY; y++)
            for(int y = 0; y < gridHeight; y++)
            {
                a3Spectrum color;

                // 当前采样位置
                a3CameraSample sampleTentFilter, sample;

                for(int z = 0; z < spp; z++)
                {
                    // 获取下一个采样位置
                    sampler->getMoreSamples(x, y, &sample, &sampleTentFilter);

                    // memory allocating
                    a3Ray ray;

                    // 生成光线
                    camera->castRay(&sampleTentFilter, &ray);

                    color += integrator->li(ray, *scene) / spp;
                }

                // 临时空间中setColor
                bufferPointer[(x + y * gridWidth) * 3 + 0] = color.x;
                bufferPointer[(x + y * gridWidth) * 3 + 1] = color.y;
                bufferPointer[(x + y * gridWidth) * 3 + 2] = color.z;
            }
        }

        // 当前buffer后期处理
        //postEffect(currentGrid);

        // 发送当前grid指定buffer
        send(currentGrid);
    }

    // already finished
    finished = true;
}

void a3IPCRenderer::postEffect(int currentGrid)
{    
    // pointer to the current grid buffer
    float* bufferPointer = gridBuffer[currentGrid];

    int gridX = (int) (currentGrid % levelX) * gridWidth;
    int gridY = (int) (currentGrid / levelX) * gridHeight;

    int gridEndX = gridX + gridWidth;
    int gridEndY = gridY + gridHeight;

    a3Log::success("Post Effct Started\n");

    if(!enableToneMapping)
    {
        // clamp color
        //#pragma omp parallel for schedule(dynamic)
        for(int x = gridX; x < gridEndX; x++)
        {
            //a3Log::info("Clamp: %8.2f \r", (double) (x - gridX) / gridWidth * 100);

            for(int y = gridY; y < gridEndY; y++)
            {
                bufferPointer[(x + y * gridWidth) * 3 + 0] = t3Math::clamp(bufferPointer[(x + y * gridWidth) * 3 + 0], 0.0f, 1.0f);
                bufferPointer[(x + y * gridWidth) * 3 + 1] = t3Math::clamp(bufferPointer[(x + y * gridWidth) * 3 + 1], 0.0f, 1.0f);
                bufferPointer[(x + y * gridWidth) * 3 + 2] = t3Math::clamp(bufferPointer[(x + y * gridWidth) * 3 + 2], 0.0f, 1.0f);
            }
        }
        a3Log::print("\n");
    }

    // --!
    // tone mapping
    a3Log::info("Tone Mapping: %s\n", enableToneMapping ? "enabled" : "disabled");
    if(enableToneMapping)
        a3ToneMapping(bufferPointer, gridWidth, gridHeight);

    //#pragma omp parallel for schedule(dynamic)
    for(int x = gridX; x < gridEndX; x++)
    {
        //a3Log::info("Gamma Correction:%s    Imaging: %8.2f \r", enableGammaCorrection ? "enabled" : "disabled", (double) x / imageWidth * 100);

        for(int y = gridY; y < gridEndY; y++)
        {
            // gamma correction
            if(enableGammaCorrection)
                a3GammaCorrection(bufferPointer[(x * gridWidth + y) * 3 + 0],
                                  bufferPointer[(x * gridWidth + y) * 3 + 1], 
                                  bufferPointer[(x * gridWidth + y) * 3 + 2]);
        }
    }

    a3Log::print("\n");
    a3Log::success("Post Effct Ended\n");
}

void a3IPCRenderer::send(int currentGrid)
{        
    // pointer to the current grid buffer
    float* bufferPointer = gridBuffer[currentGrid];
    int gridSize = gridWidth * gridHeight * 3;

    if(!ipcC2S.isFull())
    {
        // grid left-up position
        int gridX = (int) (currentGrid % levelX) * gridWidth;
        int gridY = (int) (currentGrid / levelX) * gridHeight;

        // right-down
        int gridEndX = gridX + gridWidth;
        int gridEndY = gridY + gridHeight;

        a3Log::debug("Sending Grid[%d / %d], Size:%d, Start[%d, %d], End[%d, %d]\n", currentGrid, gridCount, gridSize, gridX, gridY, gridEndX, gridEndY);
        if(gridSize > A3_GRIDBUFFER_LENGTH)
            a3Log::warning("Grid size too long\n");

        // send
        a3C2SGridBufferMessage gridMsg;

        gridMsg.currentIndex = currentGrid;
        gridMsg.gridX = gridX;
        gridMsg.gridY = gridY;

        gridMsg.gridWidth = gridSize <= A3_GRIDBUFFER_LENGTH ? gridWidth : A3_GRIDBUFFER_MAX_WIDTH;
        gridMsg.gridHeight = gridSize <= A3_GRIDBUFFER_LENGTH ? gridHeight : A3_GRIDBUFFER_MAX_HEIGHT;

        // copy buffer to msg
        memcpy(gridMsg.buffer, bufferPointer, sizeof(float) * gridSize);

        ipcC2S.enqueue(gridMsg);
    }
    else
        a3Log::warning("Full Message Queue\n");
}

//int a3IPCRenderer::getGridCount() const
//{
//    return gridCount;
//}

bool a3IPCRenderer::isFinished() const
{
    return finished;
}

a3S2CInitMessage * a3IPCRenderer::getInitMessage() const
{
    return initMsg;
}

bool a3IPCRenderer::check()
{
    if(!camera)
    {
        a3Log::error("a3IPCRenderer::camera: %d尚未分配指定\n", camera);
        return false;
    }
    else if(!camera->image)
    {
        a3Log::error("a3IPCRenderer::camera中image: %d尚未分配指定\n", camera->image);
        return false;
    }

    if(!sampler)
    {
        a3Log::error("a3IPCRenderer::sampler: %d尚未分配指定\n", sampler);
        return false;
    }

    if(!integrator)
    {
        a3Log::error("a3IPCRenderer::integrator: %d尚未分配指定\n", integrator);
        return false;
    }

    if(gridWidth <= 0 || gridHeight <= 0)
    {
        a3Log::error("a3IPCRenderer::gridWidth:%d, gridHeight: %d 数据非法\n", gridWidth, gridHeight);
        return false;
    }

    return true;
}