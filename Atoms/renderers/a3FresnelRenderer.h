#ifndef A3_FRESNELRENDERER_H
#define A3_FRESNELRENDERER_H

#include <renderers/a3Renderer.h>

class a3Sampler;
class a3Film;

// [Debug]
class a3FresnelRenderer : public a3Renderer
{
public:
    void render(const a3Scene* scene);

    bool check();

    float getFresnel(const a3Scene* scene, a3Ray* ray, const a3CameraSample* sample);

    // 采样器
    a3Sampler* sampler;

    // 相机
    a3Sensor* camera;

    // 生成的菲涅尔贴图
    a3Film* image;
};

#endif