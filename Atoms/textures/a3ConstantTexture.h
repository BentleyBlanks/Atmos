#ifndef A3_CONSTANTTEXTURE_H
#define A3_CONSTANTTEXTURE_H

#include <textures/a3Texture.h>

template <class T>
class a3ConstantTexture:public a3Texture<T>
{
public:
    a3ConstantTexture(const T value) :value(value) {}

    virtual T evaluate(float u, float v) const override
    {
        return value;
    }

    T value;
};

#endif