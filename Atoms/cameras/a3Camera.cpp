#include <cameras/a3Camera.h>
#include <core/image/a3Film.h>
#include <core/log/a3Log.h>

a3Camera::a3Camera(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                   float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
                   float focalDistance, float lensRadius,
                   a3Film* image)
                   : origin(origin), up(up), focalLength(focalLength), apreture(apretureWidth, apretureHeight), canvasDistance(canvasDistance), focalDistance(focalDistance), lensRadius(lensRadius), image(image)
{
    // 这里不直接给出fov而是间接计算
    // 详情可见 http://www.scratchapixel.com/lessons/3d-basic-rendering/3d-viewing-pinhole-camera/virtual-pinhole-camera-model
    fov.x = 2 * t3Math::atanRad(focalLength / (apretureWidth / 2));
    fov.y = 2 * t3Math::atanRad(focalLength / (apretureHeight / 2));

    canvasSize.x = canvasDistance * t3Math::tanRad(fov.x / 2);
    canvasSize.y = canvasDistance * t3Math::tanRad(fov.y / 2);

    setCameraToWorld(origin, lookat, up);
}

float a3Camera::castRay(const a3CameraSample* sample, a3Ray* ray) const
{
    a3Log::warning("Unimplemented a3Camera::generateRay() method called");

    return 0.0f;
}

void a3Camera::setCameraToWorld(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up)
{
    this->origin = origin;
    this->lookat = lookat;
    this->direction = (lookat - origin).getNormalized();

    if((up.getNormalized()).getCrossed(this->direction).length() == 0)
    {
        a3Log::error("\"up\" vector (%f, %f, %f) and viewing direction (%f, %f, %f) "
              "passed to LookAt are pointing in the same direction.  Using "
              "the identity transformation.", up.x, up.y, up.z, direction.x, direction.y,
              direction.z);
        return;
    }

    this->right = (up.getNormalized()).getCrossed(this->direction).getNormalized();
    this->up = this->direction.getCrossed(this->right);

    // 平移矩阵直接可直接作用光线原点 减少矩阵计算量
    // world to camera
    cameraToWorld._mat[0].set(this->right.x, this->right.y, this->right.z, 0);
    cameraToWorld._mat[1].set(this->up.x, this->up.y, this->up.z, 0);
    cameraToWorld._mat[2].set(this->direction.x, this->direction.y, this->direction.z, 0);
    cameraToWorld._mat[3].set(this->origin.x, this->origin.y, this->origin.z, 1);

    // camera to world
    //cameraToWorld = cameraToWorld.getInverse();
}
