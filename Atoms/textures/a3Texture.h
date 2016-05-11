#ifndef A3_TEXTURE_H
#define A3_TEXTURE_H

template<class T>
class a3Texture
{
public:
    virtual ~a3Texture() {}

    virtual T evaluate(float u, float v) const = 0;
};

#endif