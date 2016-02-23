#ifndef A3_SAMPLERRENDERER_H
#define A3_SAMPLERRENDERER_H

#include <renderers/a3Renderer.h>

class a3SamplerRenderer : public a3Renderer
{
public:
	a3SamplerRenderer();

	~a3SamplerRenderer();

	void render(const a3Scene* scene);
};

#endif