#include <lights/a3InfiniteAreaLight.h>
#include <core/a3Warp.h>
#include <core/a3Ray.h>
#include <core/image/a3ImageDecoder.h>

a3InfiniteAreaLight::a3InfiniteAreaLight(const std::string& filePath)
{
    image = new a3ImageDecoder(filePath);
}

a3InfiniteAreaLight::~a3InfiniteAreaLight()
{

}

t3Vector3f a3InfiniteAreaLight::Le(const a3Ray& ray)
{
    // 光线方向已归一化 将theta/phi归一化至[0, 1]
    float u = a3SphericalPhi(ray.direction) * T3MATH_INV_TWOPI;
    float v = a3SphericalTheta(ray.direction) * T3MATH_INV_PI;

    return image->lookup(u, v);
}

