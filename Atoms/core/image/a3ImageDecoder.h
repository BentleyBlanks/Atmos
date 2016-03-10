#ifndef A3_IMAGEDECODER_H
#define A3_IMAGEDECODER_H

#include <vector>
#include <t3Math/core/t3Vector3.h>
#include <core/a3Settings.h>

// ��֧��pngͼ���ʽ�ĵ���
class a3ImageDecoder
{
public:
    a3ImageDecoder();

    a3ImageDecoder(const std::string& filePath);

    ~a3ImageDecoder();

    void load(const std::string& filePath);

    t3Vector3f getColor(int x, int y);

    // ��������ӳ����� u, v: [0, 1]
    t3Vector3f lookup(float u, float v);

    void print();

private:
    // pimple���������������
    class a3Decoder;
    a3Decoder* decoder;
};

#endif