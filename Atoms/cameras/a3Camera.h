#ifndef A3_CAMERA_H
#define A3_CAMERA_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>

class a3Ray;
class a3CameraSample;
class a3Image;

// Զ���ü���ֱ�Ϊ0.01��1000.0
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

    // ��Ȧ����ȷ��film gate aspect ratio
    float focalLength;
    t3Vector2f apreture;

    // ���ȵ�λ
    t3Vector2f fov;

    // �洢�����Ŀ��(x: width, y:height)
    t3Vector2f canvasSize;
    // һ��Ĭ��Ϊ������������� 1.0
    float canvasDistance;

    // �������xy�����ϵ�����
    //t3Vector3f dx, dy;

    // ���࣬͸���뾶 ���ڹ��쾰��
    float focalDistance;
    float lensRadius;
};

#endif