#include <cameras/a3PerspectiveCamera.h>
#include <samples/a3CameraSample.h>
#include <t3Math/core/t3Vector3.h>
#include <samples/a3CameraSample.h>
#include <core/image/a3Film.h>
#include <core/a3Ray.h>
#include <core/a3Warp.h>
#include <core/a3Random.h>
#include <core/log/a3Log.h>
#include <core/image/a3NormalMap.h>

a3PerspectiveCamera::a3PerspectiveCamera(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                                         float focalLength, float apretureWidth, float apretureHeight, float canvasDistance,
                                         float focalDistance, float lensRadius,
                                         a3Film* image, a3NormalMap* normalMap)
                                         : a3Camera(origin, lookat, up, focalLength, apretureWidth, apretureHeight, canvasDistance, focalDistance, lensRadius, image, normalMap)
{
    random = new a3Random();

    if(image)
        a3Log::debug("Generating rendered image, width:%d, height:%d", image->width, image->height);

    if(normalMap)
        a3Log::debug("Generating normal map, width:%d, height:%d", normalMap->width, normalMap->height);
}

float a3PerspectiveCamera::castRay(const a3CameraSample* sample, a3Ray* ray) const
{
    // 不做空判断
    t3Vector2f rasterPosition(sample->imageX, sample->imageY);

    t3Vector3f cameraPosition;

    // 成像空间到摄像机空间(画布空间)的转换
    cameraPosition.x = 2 * rasterPosition.x * canvasSize.x / image->width - canvasSize.x;

    cameraPosition.y = -2 * rasterPosition.y * canvasSize.y / image->height + canvasSize.y;

    cameraPosition.z = canvasDistance;

    // 凸透镜相机模型存在景深
    if(lensRadius > 0.0f)
    {        
        // 从摄像机位置到画布上出射光线
        ray->direction = cameraPosition;
        ray->direction.normalize();

        // 在Square平面[0, 1]生成随机采样点
        float sampleU = random->randomFloat();
        float sampleV = random->randomFloat();

        t3Vector2f lens = a3SquareToUniformDisk(sampleU, sampleV);

        lens.x *= lensRadius;
        lens.y *= lensRadius;

        t3Vector3f focusPoint = ((canvasDistance + focalDistance) / ray->direction.z) * ray->direction;

        ray->origin = t3Vector3f(lens.x + origin.x, lens.y + +origin.y, origin.z);

        // camera to world
        focusPoint = focusPoint * cameraToWorld;

        ray->direction = (focusPoint - ray->origin).normalize();
    }
    else
    {
        // camera to world
        cameraPosition = cameraPosition * cameraToWorld;

        // 从摄像机位置到画布上出射光线
        ray->direction = cameraPosition;
        ray->direction.normalize();

        //// 相机空间原点
        //ray->origin = t3Vector3f::zero();

        ray->origin = origin;
        ray->direction = (cameraPosition - ray->origin).normalize();
    }  
    
    return 1.0f;
}
