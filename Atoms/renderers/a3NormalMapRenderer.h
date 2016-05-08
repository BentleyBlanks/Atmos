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

    // ������
    a3Sampler* sampler;

    // ���
    a3Sensor* camera;

    // ���ɵķ�����ͼ
    a3NormalMap* normalMap;
};

#endif