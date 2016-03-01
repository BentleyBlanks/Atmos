#ifndef A3_SAMPLERRENDERER_H
#define A3_SAMPLERRENDERER_H

#include <renderers/a3Renderer.h>

class a3Sampler;

class a3SamplerRenderer : public a3Renderer
{
public:
	a3SamplerRenderer();

	~a3SamplerRenderer();

    t3Vector3f Li(const a3Scene* scene, const a3Ray* ray, int depth, const a3CameraSample* sample, a3Intersection* intersection);

	void render(const a3Scene* scene);

    a3Sampler* sampler;

    a3Camera* camera;
};

#endif