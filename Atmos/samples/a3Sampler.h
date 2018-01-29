#pragma once

#include <core/a3Settings.h>

class a3CameraSample;

class a3Sampler
{
public:
    a3Sampler();

    // 默认均匀采样
    virtual int getMoreSamples(int x, int y, a3CameraSample* sample, a3CameraSample* sampleFilter = NULL) = 0;
};