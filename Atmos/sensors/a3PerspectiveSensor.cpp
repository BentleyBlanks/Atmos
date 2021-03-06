﻿#include <sensors/a3PerspectiveSensor.h>
#include <samples/a3CameraSample.h>
#include <t3Math/core/t3Vector3.h>
#include <samples/a3CameraSample.h>
#include <core/image/a3Film.h>
#include <core/a3Ray.h>
#include <core/a3Warp.h>
#include <core/random/a3Random.h>
#include <core/log/a3Log.h>

void checkImage(a3Film* image)
{
    if(image)
        a3Log::debug("Generating rendered image, width:%d, height:%d\n", image->width, image->height);
}

a3PerspectiveSensor::a3PerspectiveSensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                                         float focalLength, float apretureWidth, float apretureHeight, float canvasDistance,
                                         float focalDistance, float lensRadius,
                                         a3Film* image)
                                         : a3Sensor(origin, lookat, up, 
                                                    focalLength, apretureWidth, apretureHeight, canvasDistance, 
                                                    focalDistance, lensRadius, 
                                                    image)
{
    //random = new a3Random();

    checkImage(image);
}

a3PerspectiveSensor::a3PerspectiveSensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up, 
                                         float fovX, float fovY, 
                                         float focalDistance, float lensRadius, 
                                         a3Film* image)
                                         : a3Sensor(origin, lookat, up, 
                                                    1.0f, a3FovToApretureSizeDeg(fovX), a3FovToApretureSizeDeg(fovY), 1.0f, 
                                                    focalDistance, lensRadius, image)
{
    //random = new a3Random();

    checkImage(image);
}

a3PerspectiveSensor::a3PerspectiveSensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up, 
                                         float fov, 
                                         float focalDistance, float lensRadius, 
                                         a3Film* image)
                                         : a3Sensor(origin, lookat, up,
                                                    1.0f, a3FovToApretureSizeDeg(fov), a3FovToApretureSizeDeg(fov) * image->width / image->height, 1.0f,
                                                    focalDistance, lensRadius, image)
{
    //random = new a3Random();

    checkImage(image);
}

float a3PerspectiveSensor::castRay(const a3CameraSample* sample, a3Ray* ray) const
{
    // 不做空判断
    t3Vector2f rasterPosition(sample->imageX, sample->imageY);

    t3Vector3f cameraPosition;

    //// 成像空间到摄像机空间(画布空间)的转换
    //cameraPosition.x = 2 * rasterPosition.x * canvasSize.x / image->width - canvasSize.x;

    //cameraPosition.y = -2 * rasterPosition.y * canvasSize.y / image->height + canvasSize.y;

    //cameraPosition.z = canvasDistance;

    // raster to screen
    t3Vector2f screenPosition(rasterPosition.x - image->width / 2.0f, -rasterPosition.y + image->height / 2.0f);

    // screen to camera
    cameraPosition.x = 2 * canvasSize.x * screenPosition.x / image->width;

    cameraPosition.y = 2 * canvasSize.y * screenPosition.y / image->height;

    cameraPosition.z = canvasDistance;

    cameraPosition.normalize();


    // 凸透镜相机模型存在景深
    if(lensRadius > 0.0f)
    {        
        // 从摄像机位置到画布上出射光线
        ray->set(t3Vector3f::zero(), cameraPosition);

        // 在Square平面[0, 1]生成随机采样点
        float sampleU = a3Random::randomFloat();
        float sampleV = a3Random::randomFloat();

        t3Vector2f lens = a3UniformSampleDisk(sampleU, sampleV);

        lens.x *= lensRadius;
        lens.y *= lensRadius;
        
        // 需要多少个单位向量
        float t = focalDistance / ray->direction.z;
        t3Vector3f focusPoint = (*ray)(t);

        ray->origin = cameraToWorld * t3Vector3f(lens.x, lens.y, 0.0f);

        // camera to world
        focusPoint =  cameraToWorld * focusPoint;
        ray->direction = (focusPoint - ray->origin).getNormalized();
    }
    else
    {
        // camera to world
        ray->origin = cameraToWorld * t3Vector3f::zero();

        cameraPosition = cameraToWorld * cameraPosition;
        ray->direction = (cameraPosition - ray->origin).getNormalized();
    }  
    
    return 1.0f;
}
 