#include <cameras/a3PerspectiveCamera.h>
#include <samples/a3CameraSample.h>
#include <t3Math/core/t3Vector3.h>
#include <samples/a3CameraSample.h>
#include <image/a3Image.h>
#include <core/a3Ray.h>

a3PerspectiveCamera::a3PerspectiveCamera(const t3Vector3f& origin, const t3Vector3f& direction,
                                         float focalLength, float apretureWidth, float apretureHeight, float canvasDistance,
                                         a3Image* image)
: a3Camera(origin, direction, focalLength, apretureWidth, apretureHeight, canvasDistance, image)
{

}

float a3PerspectiveCamera::castRay(const a3CameraSample* sample, a3Ray* ray) const
{
    // �������ж�
    t3Vector2f rasterPosition(sample->imageX, sample->imageY);

    t3Vector3f cameraPosition;

    // ����ռ䵽������ռ�(�����ռ�)��ת��
    cameraPosition.x = 2 * rasterPosition.x * canvasSize.x / image->width - canvasSize.x;

    cameraPosition.y = -2 * rasterPosition.y * canvasSize.y / image->height + canvasSize.y;

    cameraPosition.z = canvasDistance;

    // �������λ�õ������ϳ������
    ray->direction = cameraPosition - origin;
    ray->direction.normalize();

    ray->origin = origin;

    return 1.0f;
}
