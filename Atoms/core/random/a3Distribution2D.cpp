#include <core/random/a3Distribution2D.h>
#include <core/random/a3Distribution1D.h>
#include <core/log/a3Log.h>

a3Distribution2D::a3Distribution2D(std::vector<float> data, int _width, int _height)
{
    if(_width * _height != data.size())
    {
        a3Log::error("初始化a3Distribution2D中给出了错误的数据集或宽高");
    }

    width = _width;
    height = _height;

    // 初始化条件概率分布
    // p(u|v) = f[u, v]/(\frac{1}{width}*\sum_i f[u_i, v])
    for(int i = 0; i < height; i++)
    {
        std::vector<float> row(data.begin() + i * width, data.begin() + (i + 1) * width);

        a3Distribution1D* conditional = new a3Distribution1D(row);

        conditionalDensity.push_back(conditional);
    }

    // 边缘概率分布(该位置占该行积分的比例)
    std::vector<float> marginalDensityValue; 
    marginalDensityValue.reserve(height);

    // 行积分值已在条件概率密度中计算(该行积分占全体比例)
    for(int i = 0; i < height; i++)
        marginalDensityValue.push_back(conditionalDensity[i]->functionIntegral);

    marginalDensity = new a3Distribution1D(marginalDensityValue);
}

a3Distribution2D::~a3Distribution2D()
{
    // 深释放
    for(int i = 0; i < conditionalDensity.size(); i++)
    {
        delete conditionalDensity[i];
        conditionalDensity[i] = NULL;
    }

    conditionalDensity.clear();
    
    delete marginalDensity;
    marginalDensity = NULL;
}

void a3Distribution2D::sampleContinuous(float u0, float u1, float *x0, float *x1, float *pdf)
{
    if(u0 && u1)
    {
        a3Log::error("a3Distribution2D::sampleContinuous() 参数u0, u1有误");

        *x0 = 0.0f;
        *x1 = 0.0f;

        return;
    }

    float pdfMarginal = 0.0f, pdfConditional = 0.0f;

    int index = 0;

    *x1 = marginalDensity->sampleContinuous(u0, &pdfMarginal, &index);

    // 由边缘确定的指定行
    *x0 = conditionalDensity[index]->sampleContinuous(u1, &pdfConditional);

    // 联合 = 条件 * 边缘
    *pdf = pdfConditional * pdfMarginal;
}
