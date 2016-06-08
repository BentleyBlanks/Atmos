#ifndef A3_GRIDRENDERER_H
#define A3_GRIDRENDERER_H

#include <renderers/a3Renderer.h>
#include <core/a3Settings.h>
#include <core/a3Spectrum.h>

class a3Random;
class a3Sampler;
class a3Integrator;

class a3GridRenderer : public a3Renderer
{
public:
    a3GridRenderer();
    a3GridRenderer(int spp);
    a3GridRenderer(int spp, int startX, int startY, int renderWidth, int renderHeight);
    ~a3GridRenderer();

    // 初始化需要分配的信息
    void begin();

    void end();

    // 初始化划分力度
    void setLevel(int levelX = A3_GRID_LEVELX, int levelY = A3_GRID_LEVELY);

    virtual void render(const a3Scene* scene) override;

    void postEffect();

    bool check();

    void write();

    // 采样器
    a3Sampler* sampler;

    // 相机
    a3Sensor* camera;

    // 积分器
    a3Integrator* integrator;

    // 伽马校正开关
    bool enableGammaCorrection;

    // Tone Mapping开关
    bool enableToneMapping;

    // Samples Per Pixels
    int spp;

    // color list for temp use
    a3Spectrum* colorList;

    // 局部区域划分Grid渲染
    int startX, startY;

    // 渲染区域宽高
    int renderWidth, renderHeight;

    // 划分力度
    int levelX, levelY;

    // [private]
    // 当前遍历到的网格序号
    int currentGrid;

    // 网格宽高
    int gridWidth, gridHeight;

    // 真实图像大小的宽高
    int imageWidth, imageHeight;
};

#endif