#ifndef A3_PERSPECTIVECAMERA_H
#define A3_PERSPECTIVECAMERA_H

#include <cameras/a3Camera.h>

class a3Ray;
class a3CameraSample;
class a3Image;
class a3Random;

class a3PerspectiveCamera : public a3Camera
{
public:
    a3PerspectiveCamera(const t3Vector3f& origin, const t3Vector3f& direction,
                        float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
                        float focalDistance, float lensRadius,
                        a3Image* image);

    virtual float castRay(const a3CameraSample* sample, a3Ray* ray) const;

    a3Random* random;
};

#endif