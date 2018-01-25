#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/a3Utils.h>

a3Shape::a3Shape(const t3Matrix4x4 & shapeToWorld)
    :shapeToWorld(shapeToWorld), light(NULL), bsdf(NULL), worldToShape(shapeToWorld.getInverse()), name("a3Shape")
{

}

a3Shape::~a3Shape()
{
}

bool a3Shape::intersect(const a3Ray & ray, float * t, float * u, float * v) const
{
    a3FuncNotImplementedError();

    return false;
}

float a3Shape::area() const
{
    a3FuncNotImplementedError();

    return 0.0f;
}

void a3Shape::sample(a3ShapeSamplingRecord & sRec) const
{
    a3FuncNotImplementedError();
}

float a3Shape::pdf(const a3ShapeSamplingRecord & sRec) const
{
    return 1 / area();
}

t3Vector3f a3Shape::getNormal(const t3Vector3f & hitPoint, float u, float v) const
{
    a3FuncNotImplementedError();

    return t3Vector3f::zero();
}

void a3Shape::setBSDF(a3BSDF* bsdf)
{
    if(bsdf)
        this->bsdf = bsdf;
    else
        a3NullPtrWarning("bsdf");
}

const a3BSDF* a3Shape::getBSDF() const
{
    return bsdf;
}

void a3Shape::setLight(a3Light * light)
{
    if(light)
        this->light = light;
    else
        a3NullPtrWarning("light");
}

a3Light * a3Shape::getLight() const
{
    return light;
}

bool a3Shape::isLight() const
{
    return light != NULL;
}
