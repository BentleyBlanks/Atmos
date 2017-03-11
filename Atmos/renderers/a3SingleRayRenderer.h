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

    // 采样器
    a3Sampler* sampler;

    // 相机
    a3Sensor* camera;

    // 积分器
    a3Integrator* integrator;

    // 伽马校正开关
    bool enableGammaCorrection;

    // 屏幕上需发射单一射线位置
    int singleRayX, singleRayY;
};

#endif