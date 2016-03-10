#include <lights/a3Light.h>
#include <core/a3Ray.h>

a3Light::a3Light()
{

}

t3Vector3f a3Light::Le(const a3Ray& ray)
{
    return t3Vector3f::zero();
}