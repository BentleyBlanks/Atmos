#ifndef A3_LIGHT_H
#define A3_LIGHT_H

#include <shapes/a3Sphere.h>

// ��ʱ�����͹�Դ 
class a3Light : public a3Sphere
{
public:
    a3Light(const t3Vector3f& center, const float radius = 0.0f);

    virtual void print() const;
};

#endif