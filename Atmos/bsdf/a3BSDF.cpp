#include <bsdf/a3BSDF.h>

//bool a3BSDF::isReflection() const
//{
//    return false;
//}

a3Spectrum a3BSDF::getColor(const a3Intersection& its) const
{
    // 临时写法
    if(texture)
        return texture->evaluate(its.vtu, its.vtv);
    else
        return a3Spectrum::one();
}
