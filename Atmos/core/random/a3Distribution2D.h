#ifndef A3_DISTRUBUTION2D_H
#define A3_DISTRUBUTION2D_H

#include <vector>

class a3Distribution1D;

class a3Distribution2D
{
public:
    // ��ά�����һά���Դ洢(������)
    a3Distribution2D(std::vector<float> data, int width, int height);

    ~a3Distribution2D();

    // ������ȷֲ����������(u0, u1)->(x0, x1)
    void sampleContinuous(float u0, float u1, float *x0, float *x1, float *pdf);

private:
    // ��Ե���ʷֲ� p(v)
    a3Distribution1D* marginalDensity;

    // �������ʷֲ�
    std::vector<a3Distribution1D*> conditionalDensity;
    
    // ��ά������
    int width, height;
};

#endif
