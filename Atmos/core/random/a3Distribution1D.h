#pragma once

#include <vector>

class a3Distribution1D
{
public:
    // ����һά�ֶηֲ�����
    a3Distribution1D(std::vector<float> f);

    ~a3Distribution1D();

    // inverse method
    float sampleContinuous(float u, float* pdf, int* index = NULL);

private:
    friend class a3Distribution2D;

    // ���ʷֲ����� �����ܶȺ���
    std::vector<float> f, cdf;

    // �����ֶκ����ڶ������ڵĻ���ֵ
    float functionIntegral;
};
