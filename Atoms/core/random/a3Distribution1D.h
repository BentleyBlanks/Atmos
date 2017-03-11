#ifndef A3_DISTRUBUTION1D_H
#define A3_DISTRUBUTION1D_H

#include <vector>

class a3Distribution1D
{
public:
    // 给定一维分段分布函数
    a3Distribution1D(std::vector<float> f);

    ~a3Distribution1D();

    // 给定u~U(0,1)，返回值满足指定pdf分布
    // index表明被采样点所在区间序号 原函数中比重越大序号位置处 该位置被采样几率越大
    float sampleContinuous(float u, float* pdf, int* index = NULL);

private:
    friend class a3Distribution2D;

    // 概率分布函数 概率密度函数
    std::vector<float> f, cdf;

    // 给定分段函数在定义域内的积分值
    float functionIntegral;
};

#endif