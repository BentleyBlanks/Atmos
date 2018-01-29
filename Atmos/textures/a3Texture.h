#pragma once

template<class T>
class a3Texture
{
public:
    virtual ~a3Texture() {}

    virtual T evaluate(float u, float v) const = 0;
};