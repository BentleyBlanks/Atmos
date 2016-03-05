#ifndef A3_CAMERA_H
#define A3_CAMERA_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>

class a3Ray;
class a3CameraSample;
class a3Image;

// 远近裁剪面分别为0.01与1000.0
class a3Camera
{
public:
    // focalLength + apreture -> fov
    // fov + canvasDistance -> canvasSize
    a3Camera(const t3Vector3f& origin, const t3Vector3f& direction, 
             float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
             float focalDistance, float lensRadius,
             a3Image* image);

    virtual float castRay(const a3CameraSample* sample, a3Ray* ray) const;

    t3Vector3f origin, direction;

    a3Image* image;

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

#endif