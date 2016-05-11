#ifndef A3_CHECKERBOARDTEXTURE_H
#define A3_CHECKERBOARDTEXTURE_H

#include <textures/a3ConstantTexture.h>
#include <core/a3Settings.h>
#include <t3Math/core/t3Math.h>
#include <core/a3Spectrum.h>

template<class T>
class a3CheckerBoard:public a3Texture<T>
{
public:
    a3CheckerBoard(const a3ConstantTexture<T>& t1, const a3ConstantTexture<T>& t2, int level = A3_IMAGE_CHECKERBOARD_LEVEL)
    :t1(t1), t2(t2), level(level)
    {
        if(level <= 0)
            level = A3_IMAGE_CHECKERBOARD_LEVEL;
    }

    ~a3CheckerBoard() {}

    virtual T evaluate(float u, float v) const
    {
        int s = t3Math::float2int(u * level), t = t3Math::float2int(v * level);
        if((s + t) % 2 == 0)
            return t1.evaluate(u, v);
        else
            return t2.evaluate(u, v);
    }

    a3ConstantTexture<T> t1, t2;

    // 计算中只需要1/level即可
    float level;
};

a3CheckerBoard<a3Spectrum>* a3CreateChekerBoardTexture(float t1 = 1.0f, float t2 = 0.8f)
{
    a3CheckerBoard<a3Spectrum>* texture = new a3CheckerBoard<a3Spectrum>(a3ConstantTexture<a3Spectrum>(a3Spectrum(t1)),
                                                                         a3ConstantTexture<a3Spectrum>(a3Spectrum(t2)));

    return texture;
}

#endif