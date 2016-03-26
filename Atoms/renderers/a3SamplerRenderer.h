#ifndef A3_SAMPLERRENDERER_H
#define A3_SAMPLERRENDERER_H

#include <renderers/a3Renderer.h>

class a3Random;
class a3Sampler;

class a3SamplerRenderer : public a3Renderer
{
public:
	a3SamplerRenderer();

	~a3SamplerRenderer();

    void Li(const a3Scene* scene, a3Ray* ray, int depth, t3Vector3f& color, const a3CameraSample* sample, a3Intersection* intersection);

    t3Vector3f getNormal(const a3Scene* scene, a3Ray* ray, const a3CameraSample* sample);

	void render(const a3Scene* scene);

    a3Sampler* sampler;

    a3Camera* camera;

    bool enableGammaCorrection;

    int bounces, spp;
};

#endif