#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/a3Log.h>

a3Shape::a3Shape()
{
    emission = 0.0f;

    type = A3_MATERIAL_NONE;
}

float a3Shape::intersect(const a3Ray& ray) const
{
	a3Log::warning("Unimplemented a3Shape::intersect() method called");

	return 0.0f;
}

t3Vector3f a3Shape::normal(const t3Vector3f& vector) const
{
    a3Log::warning("Unimplemented a3Shape::normal() method called");

	return t3Vector3fZero;
}

void a3Shape::setMaterial(const t3Vector3f color, const float emission, int type)
{
	this->color = color;
	this->emission = emission;
	this->type = type;
}

void a3Shape::print() const
{
#ifdef _DEBUG
    color.print("a3Shape.color");
    std::cout << "a3Shape.emission:" << emission << ", type:" << type << std::endl;
#endif 
}