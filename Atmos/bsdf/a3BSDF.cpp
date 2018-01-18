#include <bsdf/a3BSDF.h>

a3BSDF::a3BSDF() : reflectance(NULL), eta(1.0f)
{

}

a3BSDF::a3BSDF(a3Texture<a3Spectrum>* texture): reflectance(texture)
{

}

a3BSDF::~a3BSDF()
{

}
