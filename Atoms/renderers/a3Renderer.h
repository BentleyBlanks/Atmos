#ifndef A3_RENDERER_H
#define A3_RENDERER_H

#include <core/a3Scene.h>

class a3Renderer
{
public:
	a3Renderer();

	~a3Renderer();

	virtual void render(const a3Scene* scene) = 0;
};

#endif