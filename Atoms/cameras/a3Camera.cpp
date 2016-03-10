#include <cameras/a3Camera.h>
#include <core/image/a3Film.h>
#include <core/log/a3Log.h>

a3Camera::a3Camera(const t3Vector3f& origin, const t3Vector3f& direction,
                   float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
                   float focalDistance, float lensRadius,
                   a3Film* image)
                   : origin(origin), direction(direction), focalLength(focalLength), apreture(apretureWidth, apretureHeight), canvasDistance(canvasDistance), focalDistance(focalDistance), lensRadius(lensRadius), image(image)
{
    // 这里不直接给出fov而是间接计算
    // 详情可见 http://www.scratchapixel.com/lessons/3d-basic-rendering/3d-viewing-pinhole-camera/virtual-pinhole-camera-model
    fov.x = 2 * t3Math::atanRad(focalLength / (apretureWidth / 2));
    fov.y = 2 * t3Math::atanRad(focalLength / (apretureHeight / 2));

    canvasSize.x = canvasDistance * t3Math::tanRad(fov.x / 2);
    canvasSize.y = canvasDistance * t3Math::tanRad(fov.y / 2);
}

float a3Camera::castRay(const a3CameraSample* sample, a3Ray* ray) const
{
    a3Log::warning("Unimplemented a3Camera::generateRay() method called");

    return 0.0f;
}
