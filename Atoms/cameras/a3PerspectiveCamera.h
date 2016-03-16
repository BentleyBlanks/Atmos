#ifndef A3_PERSPECTIVECAMERA_H
#define A3_PERSPECTIVECAMERA_H

#include <cameras/a3Camera.h>

class a3Ray;
class a3CameraSample;
class a3Film;
class a3Random;

class a3PerspectiveCamera : public a3Camera
{
public:
    a3PerspectiveCamera(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                        float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
                        float focalDistance, float lensRadius,
                        a3Film* image, a3NormalMap* normalMap = NULL);

    virtual float castRay(const a3CameraSample* sample, a3Ray* ray) const;

    a3Random* random;
};

#endif