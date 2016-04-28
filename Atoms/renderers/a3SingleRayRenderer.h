#ifndef A3_SINGLERAYRENDERER_H
#define A3_SINGLERAYRENDERER_H

#include <renderers/a3Renderer.h>

class a3Sampler;
class a3Integrator;

// [Debug]
class a3SingleRayRenderer : public a3Renderer
{
public:
    a3SingleRayRenderer();

    a3SingleRayRenderer(int singleRayX, int singleRayY);

    ~a3SingleRayRenderer();

    void render(const a3Scene* scene);

    bool check();

    // ������
    a3Sampler* sampler;

    // ���
    a3Sensor* camera;

    // ������
    a3Integrator* integrator;

    // ٤��У������
    bool enableGammaCorrection;

    // ��Ļ���跢�䵥һ����λ��
    int singleRayX, singleRayY;
};

#endif