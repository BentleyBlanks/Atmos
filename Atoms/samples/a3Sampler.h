#ifndef A3_SAMPLER_H
#define A3_SAMPLER_H

class a3CameraSample;

class a3Sampler
{
public:
    a3Sampler(int xStart, int yStart, int xEnd, int yEnd, int spp);

    // 默认均匀采样
    int getSample(const int x, const int y, a3CameraSample *sample);

private:
    // Deprecated
    const int samplesPerPixel;
    
    const int xStart, yStart, xEnd, yEnd;

    // 当前采样位置
    int x, y;
};

#endif