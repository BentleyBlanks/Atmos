#pragma once

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>
#include <t3Math/core/t3Matri4x4.h>

class a3Ray;
class a3CameraSample;
class a3Film;

// 远近裁剪面分别为0.01与1000.0
class a3Sensor
{
public:
    // focalLength + apreture -> fov
    // fov + canvasDistance -> canvasSize
    a3Sensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
             float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
             float focalDistance, float lensRadius,
             a3Film* image);

    virtual float castRay(const a3CameraSample* sample, a3Ray* ray) const = 0;

    void setCameraToWorld(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up);

    void print();

    // camera's position / eye's position
    t3Vector3f origin;

    // camera coordinate system
    t3Vector3f direction, up, right, lookat;
    // （同一坐标原点）从相机坐标系转换到世界坐标系
    t3Matrix4x4 cameraToWorld, rasterToCamera, cameraToScreen;

    a3Film* image;

    // 光圈用于确定film gate aspect ratio
    float focalLength;
    t3Vector2f apreture;

    // 弧度单位
    t3Vector2f fov;

    // 存储画布的宽高(x: width, y:height)
    t3Vector2f canvasSize;
    // 一般默认为画布距离摄像机 1.0
    float canvasDistance;

    // 摄像机在xy方向上的增量
    //t3Vector3f dx, dy;

    // 焦距，透镜半径 用于构造景深
    float focalDistance;
    float lensRadius;
};