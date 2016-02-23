#ifndef A3_CAMERA_H
#define A3_CAMERA_H

#include <t3Math/core/t3Vector3.h>

class a3Image;

// Զ���ü���ֱ�Ϊ0.01��1000.0
class a3Camera
{
public:
    a3Camera(const t3Vector3f& origin, const t3Vector3f& direction, float fov, a3Image* image);

    t3Vector3f origin, direction;

    a3Image* image;

private:
    float fov;

    // �������xy�����ϵ�����
    t3Vector3f dx, dy;
};

#endif