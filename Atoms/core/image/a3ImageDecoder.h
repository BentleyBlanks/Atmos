#ifndef A3_IMAGEDECODER_H
#define A3_IMAGEDECODER_H

#include <vector>
#include <t3Math/core/t3Vector3.h>
#include <core/a3Settings.h>

class a3ImageDecoder
{
public:
    a3ImageDecoder();

    a3ImageDecoder(const std::string& filePath);

    ~a3ImageDecoder();

    bool load(const std::string& filePath);

    bool isLoaded() const;

    int getWidth() const;

    int getHeight() const;

    t3Vector3f getColor(int x, int y) const;

    // 纹理坐标映射查找 u, v: [0, 1]
    t3Vector3f lookup(float u, float v) const;

    void print();
private:
    // pimple方便更换第三方库
    class a3Decoder;
    a3Decoder* decoder;

    bool bLoaded;
};

#endif