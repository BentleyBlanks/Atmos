#ifndef A3_IPCRENDERER_H
#define A3_IPCRENDERER_H

#include <renderers/a3Renderer.h>
#include <core/a3Settings.h>
#include <core/a3Spectrum.h>

#include <core/messageQueue/a3Message.h>
#include <core/messageQueue/a3MessageQueueIPC.h>

class a3Random;
class a3Sampler;
class a3Integrator;

// 无状态 render线程安全
class a3IPCRenderer : public a3Renderer
{
public:
    a3IPCRenderer();
    a3IPCRenderer(int spp);
    ~a3IPCRenderer();

    // 等待IPC Server直到init消息
    void waiting();

    // 初始化划分力度
    void setLevel(int levelX = A3_GRID_LEVELX, int levelY = A3_GRID_LEVELY);

    void setCurrentGrid(int currentGrid);

    // 渲染完毕自动IPC发送buffer
    virtual void render(const a3Scene* scene) override;

    int getGridCount() const;

    bool isFinished() const;

    a3Sampler* sampler;

    a3Sensor* camera;

    a3Integrator* integrator;

    // 伽马校正 Tone Mapping开关
    bool enableGammaCorrection, enableToneMapping;

    int spp;

private:    
    // 根据IPC的init信息初始化需要分配的信息
    void init();

    // 发送当前网格ID指向的buffer
    void send();

    void postEffect();

    bool check();

    // grid buffer pointer
    float** gridBuffer;

    // 真实图像大小的宽高
    int imageWidth, imageHeight;

    // 划分力度
    int levelX, levelY;

    bool finished;

    // 当前遍历到的网格序号 网格数量
    int currentGrid, gridCount;

    // 网格宽高
    int gridWidth, gridHeight;

    // Message Queue
    a3MessageQueueIPC ipcC2S, ipcS2C;
};

#endif
