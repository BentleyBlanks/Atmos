#ifndef A3_INFINITEAREA_LIGHT_H
#define A3_INFINITEAREA_LIGHT_H

#include <lights/a3Light.h>

class a3ImageDecoder;

class a3InfiniteAreaLight : public a3Light
{
public:
    a3InfiniteAreaLight(const std::string& filePath);

    ~a3InfiniteAreaLight();

    virtual t3Vector3f Le(const a3Ray& ray);

    a3ImageDecoder* image;
};

#endif 
