#pragma once

#include <vector>

class a3Distribution1D
{
public:
    // 给定一维分段分布函数
    a3Distribution1D(std::vector<float> f);

    ~a3Distribution1D();

    // inverse method
    float sampleContinuous(float u, float* pdf, int* index = NULL);

private:
    friend class a3Distribution2D;

    // 概率分布函数 概率密度函数
    std::vector<float> f, cdf;

    // 给定分段函数在定义域内的积分值
    float functionIntegral;
};
