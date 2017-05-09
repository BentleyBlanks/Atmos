#ifndef A3_IMAGETEXTURE_H
#define A3_IMAGETEXTURE_H

#include <core/log/a3Log.h>
#include <textures/a3Texture.h>
#include <core/a3Spectrum.h>
#include <core/image/a3ImageDecoder.h>

template<class T>
class a3ImageTexture:public a3Texture<T>
{
public:
    a3ImageTexture(const char* filePath)
    {
        decoder = new a3ImageDecoder();

        if(decoder->load(filePath))
            a3Log::success("Image Texture Created Succeed\n");
    }

    virtual ~a3ImageTexture()
    {
        A3_SAFE_DELETE(decoder);
    }

    virtual T evaluate(float u, float v) const
    {
        return decoder->lookup(u, v);
    }

    a3ImageDecoder* decoder;
};

inline a3ImageTexture<a3Spectrum>* a3CreateImageTexture(const char* filePath)
{
    a3ImageTexture<a3Spectrum>* texture = new a3ImageTexture<a3Spectrum>(filePath);

    return texture;
}

#endif
