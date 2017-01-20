#include <renderers/a3GridRenderer.h>
#include <core/a3Ray.h>
#include <core/image/a3Film.h>
#include <core/log/a3Log.h>
#include <core/a3Warp.h>
#include <core/a3Random.h>
#include <core/image/a3NormalMap.h>

#include <sensors/a3Sensor.h>
#include <lights/a3Light.h>
#include <shapes/a3Shape.h>
#include <integrator/a3Integrator.h>

#include <samples/a3Sampler.h>
#include <samples/a3CameraSample.h>

#include <Common/t3Timer.h>

a3GridRenderer::a3GridRenderer()
    : spp(128), sampler(NULL), camera(NULL),
    enableGammaCorrection(true), enableToneMapping(false),
    startX(0), startY(0), renderWidth(0), renderHeight(0), currentGrid(0),
    gridWidth(0), gridHeight(0),
    levelX(A3_GRID_LEVELX), levelY(A3_GRID_LEVELY),
    finished(false),
    colorList(NULL)
{

}

a3GridRenderer::a3GridRenderer(int spp, int startX, int startY, int renderWidth, int renderHeight)
    : spp(spp), sampler(NULL), camera(NULL),
    enableGammaCorrection(true), enableToneMapping(false),
    startX(0), startY(0), renderWidth(0), renderHeight(renderWidth), currentGrid(renderHeight),
    gridWidth(0), gridHeight(0),
    levelX(A3_GRID_LEVELX), levelY(A3_GRID_LEVELY),
    finished(false),
    colorList(NULL)
{

}

a3GridRenderer::a3GridRenderer(int spp)
    : spp(spp), sampler(NULL), camera(NULL),
    enableGammaCorrection(true), enableToneMapping(false),
    startX(0), startY(0), renderWidth(0), renderHeight(0), currentGrid(0),
    gridWidth(0), gridHeight(0),
    levelX(A3_GRID_LEVELX), levelY(A3_GRID_LEVELY),
    finished(false),
    colorList(NULL)
{

}

a3GridRenderer::~a3GridRenderer()
{

}

void a3GridRenderer::begin()
{
    finished = false;
    
    imageWidth = camera->image->width;
    imageHeight = camera->image->height;

    if(renderWidth <= 0)
        renderWidth = camera->image->width;
    if(renderHeight <= 0)
        renderHeight = camera->image->height;

    colorList = new a3Spectrum[camera->image->width * camera->image->height]();

    // 初始化网格渲染信息
    gridWidth = renderWidth / levelX;
    if(renderWidth % levelX != 0)
        a3Log::warning("a3GridRenderer 网格在x轴上无法完整划分，渲染结果中可能出现黑边\n");

    gridHeight = renderHeight / levelY;
    if(renderHeight % levelY != 0)
        a3Log::warning("a3GridRenderer 网格在y轴上无法完整划分，渲染结果中可能出现黑边\n");

    return;
}

void a3GridRenderer::end()
{
    postEffect();

    write();
}

void a3GridRenderer::setLevel(int levelX, int levelY)
{
    if(levelX > 0)
        this->levelX = levelX;
    else
        this->levelX = A3_GRID_LEVELX;

    if(levelY > 0)
        this->levelY = levelY;
    else
        this->levelX = A3_GRID_LEVELY;
}

void a3GridRenderer::render(const a3Scene* scene)
{
    // 已结束
    if(currentGrid >= levelY * levelX)
    {
        finished = true;
        return;
    }

    if(!check())
    {
        a3Log::warning("Grid Rendering Start Failed\n");
        return;
    }
    else
        a3Log::success("Grid Rendering Started\n");

    // 局部Grid渲染
    int gridX = startX + (int) (currentGrid % levelX) * gridWidth;
    int gridY = startY + (int) (currentGrid / levelX) * gridHeight;

    int gridEndX = gridX + gridWidth;
    int gridEndY = gridY + gridHeight;

#pragma omp parallel for schedule(dynamic)
    for(int x = gridX; x < gridEndX; x++)
    {
        progress = (float) (x - gridX) / gridWidth;
        a3Log::info("Grid[%d/%d] SPP:%d Rendering: %8.2f \r", currentGrid + 1, levelX * levelY, spp, progress * 100);
        for(int y = gridY; y < gridEndY; y++)
        {
            a3Spectrum color;

            // 当前采样位置
            a3CameraSample sampleTentFilter, sample;

            for(int z = 0; z < spp; z++)
            {
                // 获取下一个采样位置
                sampler->getMoreSamples(x, y, &sample, &sampleTentFilter);

                // memory allocating
                a3Ray ray;

                // 生成光线
                camera->castRay(&sampleTentFilter, &ray);

                color += integrator->li(ray, *scene) / spp;
            }

            // 临时空间中setColor
            colorList[x + y * camera->image->width] = color;
        }
    }

    currentGrid++;

    a3Log::print("\n");
}

void a3GridRenderer::postEffect()
{
    int renderEndX = startX + renderWidth;
    int renderEndY = startY + renderHeight;

    int width = camera->image->width;
    int height = camera->image->height;

    a3Log::success("Post Effct Started\n");

    if(!enableToneMapping)
    {
        // clamp color
        //#pragma omp parallel for schedule(dynamic)
        for(int x = startX; x < renderEndX; x++)
        {
            a3Log::info("Clamp: %8.2f \r", (double) (x - startX) / renderWidth * 100);

            for(int y = startY; y < renderEndY; y++)
            {
                a3Spectrum& color = colorList[x + y * width];

                color.x = t3Math::clamp(color.x, 0.0f, 1.0f);
                color.y = t3Math::clamp(color.y, 0.0f, 1.0f);
                color.z = t3Math::clamp(color.z, 0.0f, 1.0f);
            }
        }
        a3Log::print("\n");
    }

    // tone mapping
    a3Log::info("Tone Mapping: %s\n", enableToneMapping ? "enabled" : "disabled");
    if(enableToneMapping)
        a3ToneMapping(colorList, startX, startY, renderWidth, renderHeight, width, height);

    //#pragma omp parallel for schedule(dynamic)
    for(int x = startX; x < renderEndX; x++)
    {
        a3Log::info("Gamma Correction:%s    Imaging: %8.2f \r", enableGammaCorrection ? "enabled" : "disabled", (double) (x - startX) / renderWidth * 100);

        for(int y = startY; y < renderEndY; y++)
        {
            a3Spectrum& color = colorList[x + y * width];

            // gamma correction
            if(enableGammaCorrection)
                a3GammaCorrection(color);

            camera->image->addColor(x, y, color);
        }
    }

    a3Log::print("\n");
    a3Log::success("Post Effct Ended\n");
}

void a3GridRenderer::write()
{
    // 保存真实渲染图像文件
    camera->image->write();
}

bool a3GridRenderer::isFinished()
{
    return finished;
}

bool a3GridRenderer::check()
{
    if(!camera)
    {
        a3Log::error("a3GridRenderer::render()前camera: %d尚未分配指定\n", camera);
        return false;
    }
    else if(!camera->image)
    {
        a3Log::error("a3GridRenderer::render()前camera中image: %d尚未分配指定\n", camera);
        return false;
    }

    if(!sampler)
    {
        a3Log::error("a3GridRenderer::render()前sampler: %d尚未分配指定\n", sampler);
        return false;
    }

    if(!integrator)
    {
        a3Log::error("a3GridRenderer::render()前integrator: %d尚未分配指定\n", integrator);
        return false;
    }

    if(gridWidth <= 0 || gridHeight <= 0)
    {
        a3Log::error("a3GridRenderer::render()前gridWidth:%d, gridHeight: %d 数据非法\n", gridWidth, gridHeight);
        return false;
    }

    return true;
}

