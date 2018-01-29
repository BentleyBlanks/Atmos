#pragma once

#include <renderers/a3Renderer.h>
#include <core/a3Settings.h>
#include <core/a3Spectrum.h>

#include <core/messageQueue/a3Message.h>
#include <core/messageQueue/a3MessageQueueIPC.h>

class a3Random;
class a3Sampler;
class a3Integrator;
struct a3S2CInitMessage;

// ��״̬ render�̰߳�ȫ
class a3IPCRenderer : public a3Renderer
{
public:
    a3IPCRenderer();
    ~a3IPCRenderer();

    // �ȴ�IPC Serverֱ��init��Ϣ
    void waiting();

    // need to called before render(), and after wating()
    void init();

    // ��Ⱦ����Զ�IPC����buffer
    virtual void render(const a3Scene* scene) override;

    // ��ʼ����������
    //void setLevel(int levelX = A3_GRID_LEVELX, int levelY = A3_GRID_LEVELY);

    //void setCurrentGrid(int currentGrid);

    //int getGridCount() const;

    bool isFinished() const;

    a3S2CInitMessage* getInitMessage() const;

private:
    // ����IPC��init��Ϣ��ʼ����Ҫ�������Ϣ
    //void initScene();
    void initRenderer();
    void initBuffer();

    // ���͵�ǰ����IDָ���buffer
    void sendRays(const a3Ray& ray);

    void sendBuffer(const a3Ray& ray, int currentGrid);

    void postEffect(int currentGrid);

    bool check();

    // would allocated by IPC
    a3Sampler* sampler;

    a3Sensor* camera;

    a3Integrator* integrator;

    //a3Scene* scene;

    // ٤��У�� Tone Mapping����
    bool enableGammaCorrection, enableToneMapping;

    int spp;

    // grid buffer pointer
    float** gridBuffer;

    // ��ʵͼ���С�Ŀ��
    int imageWidth, imageHeight;

    // ��������
    int levelX, levelY;

    bool finished;

    // ��ǰ��������������� ��������
    // int currentGrid;
    int gridCount;

    // ������
    int gridWidth, gridHeight;

    // Message Queue
    a3MessageQueueIPC ipcC2S, ipcS2C;
    a3S2CInitMessage* initMsg;
};
