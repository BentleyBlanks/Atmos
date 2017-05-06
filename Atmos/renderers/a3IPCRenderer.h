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

// ��״̬ render�̰߳�ȫ
class a3IPCRenderer : public a3Renderer
{
public:
    a3IPCRenderer();
    a3IPCRenderer(int spp);
    ~a3IPCRenderer();

    // �ȴ�IPC Serverֱ��init��Ϣ
    void waiting();

    // ��ʼ����������
    void setLevel(int levelX = A3_GRID_LEVELX, int levelY = A3_GRID_LEVELY);

    void setCurrentGrid(int currentGrid);

    // ��Ⱦ����Զ�IPC����buffer
    virtual void render(const a3Scene* scene) override;

    int getGridCount() const;

    bool isFinished() const;

    a3Sampler* sampler;

    a3Sensor* camera;

    a3Integrator* integrator;

    // ٤��У�� Tone Mapping����
    bool enableGammaCorrection, enableToneMapping;

    int spp;

private:    
    // ����IPC��init��Ϣ��ʼ����Ҫ�������Ϣ
    void init();

    // ���͵�ǰ����IDָ���buffer
    void send();

    void postEffect();

    bool check();

    // grid buffer pointer
    float** gridBuffer;

    // ��ʵͼ���С�Ŀ��
    int imageWidth, imageHeight;

    // ��������
    int levelX, levelY;

    bool finished;

    // ��ǰ��������������� ��������
    int currentGrid, gridCount;

    // ������
    int gridWidth, gridHeight;

    // Message Queue
    a3MessageQueueIPC ipcC2S, ipcS2C;
};

#endif
