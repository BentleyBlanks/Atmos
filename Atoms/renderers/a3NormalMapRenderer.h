#ifndef A3_NORMALMAPRENDERER_H
#define A3_NORMALMAPRENDERER_H

#include <renderers/a3Renderer.h>

class a3NormalMap;
class a3Sampler;

// [Debug]
class a3NormalMapRenderer : public a3Renderer
{
public:
    void render(const a3Scene* scene);

    bool check();

    t3Vector3f getNormal(const a3Scene* scene, a3Ray* ray, const a3CameraSample* sample);

    // 采样器
    a3Sampler* sampler;

    // 相机
    a3Sensor* camera;

    // 生成的法线贴图
    a3NormalMap* normalMap;
};

#endif