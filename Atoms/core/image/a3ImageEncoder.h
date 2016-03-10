#ifndef A3_IMAGEENCODER_H
#define A3_IMAGEENCODER_H

#include <core/log/a3Log.h>
#include <t3Math/core/t3Vector3.h>

// ʵ�ʵ�ͼ����뱣����
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
    // pimple���������������
    class a3Encoder;
    a3Encoder* encoder;
};

#endif