#pragma once

#include <samples/a3Sampler.h>

class a3RandomSampler : public a3Sampler
{
public:
    a3RandomSampler();

    virtual int getMoreSamples(int x, int y, a3CameraSample* sample, a3CameraSample* sampleFilter = NULL);

    // 当前采样点
    int x, y;
};