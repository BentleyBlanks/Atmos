#ifndef A3_CAMERA_H
#define A3_CAMERA_H

#include <t3Math/core/t3Vector3.h>

class a3Image;

// 远近裁剪面分别为0.01与1000.0
class a3Camera
{
public:
    a3Camera(const t3Vector3f& origin, const t3Vector3f& direction, float fov, a3Image* image);

    t3Vector3f origin, direction;

    a3Image* image;

private:
    float fov;

    // 摄像机在xy方向上的增量
    t3Vector3f dx, dy;
};

#endif