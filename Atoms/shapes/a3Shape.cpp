#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/log/a3Log.h>

a3Shape::a3Shape()
{
    emission.set(0, 0, 0);

    type = A3_MATERIAL_NONE;
}

float a3Shape::intersect(const a3Ray& ray) const
{
	a3Log::warning("Unimplemented a3Shape::intersect() method called");

	return 0.0f;
}

t3Vector3f a3Shape::getNormal(const t3Vector3f& vector) const
{
    a3Log::warning("Unimplemented a3Shape::getNormal() method called");

	return t3Vector3f::zero();
}

void a3Shape::setMaterial(const t3Vector3f color, const t3Vector3f emission, int type)
{
	this->color = color;
	this->emission = emission;
	this->type = type;
}

void a3Shape::print() const
{
#ifdef _DEBUG
    color.print("a3Shape.color");
    emission.print("a3Shape.emission:");
    std::cout << ", type:" << type << std::endl;
#endif 
}