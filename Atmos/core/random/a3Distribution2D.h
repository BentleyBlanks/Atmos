#ifndef A3_DISTRUBUTION2D_H
#define A3_DISTRUBUTION2D_H

#include <vector>

class a3Distribution1D;

class a3Distribution2D
{
public:
    // 二维数组的一维线性存储(行优先)
    a3Distribution2D(std::vector<float> data, int width, int height);

    ~a3Distribution2D();

    // 满足均匀分布的随机变量(u0, u1)->(x0, x1)
    void sampleContinuous(float u0, float u1, float *x0, float *x1, float *pdf);

private:
    // 边缘概率分布 p(v)
    a3Distribution1D* marginalDensity;

    // 条件概率分布
    std::vector<a3Distribution1D*> conditionalDensity;
    
    // 二维数组宽高
    int width, height;
};

#endif
