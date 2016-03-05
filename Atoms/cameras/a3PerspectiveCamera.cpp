#include <cameras/a3PerspectiveCamera.h>
#include <samples/a3CameraSample.h>
#include <t3Math/core/t3Vector3.h>
#include <samples/a3CameraSample.h>
#include <core/image/a3Image.h>
#include <core/a3Ray.h>
#include <core/a3Warp.h>
#include <core/a3Random.h>

a3PerspectiveCamera::a3PerspectiveCamera(const t3Vector3f& origin, const t3Vector3f& direction,
                                         float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
                                         float focalDistance, float lensRadius,
                                         a3Image* image)
                                         : a3Camera(origin, direction, focalLength, apretureWidth, apretureHeight, canvasDistance, focalDistance, lensRadius, image)
{
    random = new a3Random();
}

float a3PerspectiveCamera::castRay(const a3CameraSample* sample, a3Ray* ray) const
{
    // 不做空判断
    t3Vector2f rasterPosition(sample->imageX, sample->imageY);

    t3Vector3f cameraPosition;

    // 成像空间到摄像机空间(画布空间)的转换
    cameraPosition.x = 2 * rasterPosition.x * canvasSize.x / image->width - canvasSize.x + origin.x;

    cameraPosition.y = -2 * rasterPosition.y * canvasSize.y / image->height + canvasSize.y + origin.y;

    cameraPosition.z = canvasDistance + origin.z;

    // 从摄像机位置到画布上出射光线
    ray->direction = cameraPosition - origin;
    ray->direction.normalize();

    ray->origin = origin;

    // 凸透镜相机模型存在景深
    if(lensRadius > 0.0f)
    {
        // 在Square平面[0, 1]生成随机采样点
        float sampleU = random->randomFloat();
        float sampleV = random->randomFloat();

        t3Vector2f lens = squareToUniformDisk(sampleU, sampleV);

        lens.x *= lensRadius;
        lens.y *= lensRadius;

        t3Vector3f focusPoint = origin + ((canvasDistance + focalDistance) / ray->direction.z) * ray->direction;

        ray->origin = t3Vector3f(lens.x + origin.x, lens.y + origin.y, origin.z);

        ray->direction = focusPoint - ray->origin;
        ray->direction.normalize();
    }

    return 1.0f;
}
