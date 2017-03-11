#ifndef A3_DISTRUBUTION1D_H
#define A3_DISTRUBUTION1D_H

#include <vector>

class a3Distribution1D
{
public:
    // ����һά�ֶηֲ�����
    a3Distribution1D(std::vector<float> f);

    ~a3Distribution1D();

    // ����u~U(0,1)������ֵ����ָ��pdf�ֲ�
    // index����������������������� ԭ�����б���Խ�����λ�ô� ��λ�ñ���������Խ��
    float sampleContinuous(float u, float* pdf, int* index = NULL);

private:
    friend class a3Distribution2D;

    // ���ʷֲ����� �����ܶȺ���
    std::vector<float> f, cdf;

    // �����ֶκ����ڶ������ڵĻ���ֵ
    float functionIntegral;
};

#endif