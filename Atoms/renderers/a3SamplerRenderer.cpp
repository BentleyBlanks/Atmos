#include <renderers/a3SamplerRenderer.h>

a3SamplerRenderer::a3SamplerRenderer()
{

}

a3SamplerRenderer::~a3SamplerRenderer()
{

}

void a3SamplerRenderer::render(const a3Scene* scene)
{
    // ������Ⱦ���ж�������

    // |Task.run()| |Task.run()| ... |Task.run()|

        // sample = RandomSampler.getSubSampler()

        // ray = Camera.generateRayDiffererntial(sample)

        // intersection = Scene.Intersect(ray)

        // spectrum = Intergrator.Li(intersection)

        // File.AddSample(spectrum, sample)

        // Reporter.Update()
}