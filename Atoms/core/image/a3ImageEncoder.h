#ifndef A3_IMAGEENCODER_H
#define A3_IMAGEENCODER_H

#include <core/log/a3Log.h>
#include <t3Math/core/t3Vector3.h>

// 实际的图像编码保存器
class a3ImageEncoder
{
public:
    a3ImageEncoder(unsigned width, unsigned height, a3ImageType type);

    ~a3ImageEncoder();

    void setColor(int x, int y, const t3Vector3f& L);

    void write(std::string& fileName);

    // ppm file format
    t3Vector3f **pixels;

    a3ImageType type;

private:
    // pimple方便更换第三方库
    class a3Encoder;
    a3Encoder* encoder;
};

#endif