#ifndef A3_CAMERA_H
#define A3_CAMERA_H

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>
#include <t3Math/core/t3Matri4x4.h>

class a3Ray;
class a3CameraSample;
class a3Film;
class a3NormalMap;

// Զ���ü���ֱ�Ϊ0.01��1000.0
class a3Camera
{
public:
    // focalLength + apreture -> fov
    // fov + canvasDistance -> canvasSize
    a3Camera(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
             float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
             float focalDistance, float lensRadius,
             a3Film* image, a3NormalMap* normalMap = NULL);

    virtual float castRay(const a3CameraSample* sample, a3Ray* ray) const;

    void setCameraToWorld(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up);

    // camera's position / eye's position
    t3Vector3f origin;

    // camera coordinate system
    t3Vector3f direction, up, right, lookat;
    // ��ͬһ����ԭ�㣩���������ϵת������������ϵ
    t3Matrix4x4 cameraToWorld;

    a3Film* image;
    a3NormalMap* normalMap;

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