#pragma once

#include <sensors/a3Sensor.h>

class a3Ray;
class a3CameraSample;
class a3Film;
class a3Random;

// 暂时与Thinlens合并
class a3PerspectiveSensor : public a3Sensor
{
public:
    // fov(度)与屏幕宽高比一致
    a3PerspectiveSensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                        float fov,
                        float focalDistance, float lensRadius,
                        a3Film* image);

    // fov(度)
    a3PerspectiveSensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                        float fovX, float fovY,
                        float focalDistance, float lensRadius,
                        a3Film* image);

    a3PerspectiveSensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                        float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
                        float focalDistance, float lensRadius,
                        a3Film* image);

    virtual float castRay(const a3CameraSample* sample, a3Ray* ray) const;

    //a3Random* random;
};