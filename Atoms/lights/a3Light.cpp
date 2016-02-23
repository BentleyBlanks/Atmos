#include <lights/a3Light.h>
#include <core/a3Ray.h>

a3Light::a3Light(const t3Vector3f& center, const float radius) :a3Sphere(center, radius)
{

}

void a3Light::print() const
{
#ifdef _DEBUG
    center.print("a3Light:center");

    std::cout << "a3Light:radius:" << radius << std::endl;

    a3Shape::print();
#endif
}