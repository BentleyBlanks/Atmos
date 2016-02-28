#ifndef A3_SAMPLER_H
#define A3_SAMPLER_H

class a3CameraSample;

class a3Sampler
{
public:
    a3Sampler(int xStart, int yStart, int xEnd, int yEnd, int spp);

    // Ĭ�Ͼ��Ȳ���
    int getSample(const int x, const int y, a3CameraSample *sample);

private:
    // Deprecated
    const int samplesPerPixel;
    
    const int xStart, yStart, xEnd, yEnd;

    // ��ǰ����λ��
    int x, y;
};

#endif