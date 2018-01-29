#pragma once

#include <renderers/a3Renderer.h>
#include <core/a3Spectrum.h>

class a3Random;
class a3Sampler;
class a3Integrator;

class a3SamplerRenderer : public a3Renderer
{
public:
	a3SamplerRenderer();

    a3SamplerRenderer(int spp);

	~a3SamplerRenderer();

    bool begin();

	void render(const a3Scene* scene);

    void end();

    bool check();

    void postEffect();

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
};