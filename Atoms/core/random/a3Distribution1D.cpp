#include <core/random/a3Distribution1D.h>
#include <stdlib.h>
#include <core/log/a3Log.h>

// --!�����PBRT-montecarlo.cpp
a3Distribution1D::a3Distribution1D(std::vector<float> _f)
{
    if(_f.size() <= 0)
    {
        a3Log::error("��ʼ��a3Distribution1D���ṩ�˿յ����ݼ�");
    }

    // ȫ���ݿ���
    f.swap(_f);

    cdf.resize(f.size() + 1);

    cdf[0] = 0.0f;

    int num = f.size();
    for(int i = 1; i <= num; i++)
        cdf[i] = cdf[i - 1] + f[i - 1] / num;

    functionIntegral = cdf[num];
    
    // ȫ0
    if(functionIntegral == 0.0f)
    {
        // �ٶ�������ֵ������ȷֲ�
        for(int i = 1; i <= num; i++)
            // б��Ϊ1/N��ֱ��
            cdf[i] = (float)(i) / (float)(num);
    }
    else
    {
        // ��һ��
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
    // ���Ҹ���u��������
    for(int i = 0; i < f.size(); i++)
    {
        if(cdf[i] <= u && cdf[i + 1] > u)
            index = i;
    }

    if(index == -1)
    {
        a3Log::error("a3Distribution1D::sampleContinuous() ����u����");
        return 0.0f;
    }

    if(offset)
        *offset = index;

    // �ֲ�ת���� x��������ֶκ����ֲ���
    float x = (index + (u - cdf[index]) / (cdf[index + 1] - cdf[index])) / f.size();

    if(pdf)
        *pdf = f[index] / functionIntegral;

    return x;
}
