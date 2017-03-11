#include <core/random/a3Distribution1D.h>
#include <stdlib.h>
#include <core/log/a3Log.h>

// --!借鉴自PBRT-montecarlo.cpp
a3Distribution1D::a3Distribution1D(std::vector<float> _f)
{
    if(_f.size() <= 0)
    {
        a3Log::error("初始化a3Distribution1D中提供了空的数据集");
    }

    // 全内容拷贝
    f.swap(_f);

    cdf.resize(f.size() + 1);

    cdf[0] = 0.0f;

    int num = f.size();
    for(int i = 1; i <= num; i++)
        cdf[i] = cdf[i - 1] + f[i - 1] / num;

    functionIntegral = cdf[num];
    
    // 全0
    if(functionIntegral == 0.0f)
    {
        // 假定给定数值满足均匀分布
        for(int i = 1; i <= num; i++)
            // 斜率为1/N的直线
            cdf[i] = (float)(i) / (float)(num);
    }
    else
    {
        // 归一化
        for(int i = 1; i <= num; i++)
            cdf[i] /= functionIntegral;
    }
}

a3Distribution1D::~a3Distribution1D()
{

}

float a3Distribution1D::sampleContinuous(float u, float *pdf, int* offset)
{
    int index = -1;
    // 查找给定u所在区间
    for(int i = 0; i < f.size(); i++)
    {
        if(cdf[i] <= u && cdf[i + 1] > u)
            index = i;
    }

    if(index == -1)
    {
        a3Log::error("a3Distribution1D::sampleContinuous() 参数u有误");
        return 0.0f;
    }

    if(offset)
        *offset = index;

    // 分布转换后 x满足给定分段函数分布律
    float x = (index + (u - cdf[index]) / (cdf[index + 1] - cdf[index])) / f.size();

    if(pdf)
        *pdf = f[index] / functionIntegral;

    return x;
}
