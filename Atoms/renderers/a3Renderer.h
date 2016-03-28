#ifndef A3_RENDERER_H
#define A3_RENDERER_H

#include <t3Math/core/t3Vector3.h>

class a3Scene;
class a3CameraSample;
class a3Camera;
class a3Ray;
class a3Intersection;

class a3Renderer
{
public:
	a3Renderer();

	~a3Renderer();

    virtual void Li(const a3Scene* scene, const a3Ray* ray, int depth, t3Vector3f& color, const a3CameraSample* sample, a3Intersection* intersection)= 0;

    virtual void render(const a3Scene* scene) = 0;
};

#endif