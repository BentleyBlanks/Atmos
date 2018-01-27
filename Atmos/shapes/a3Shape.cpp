#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/a3Utils.h>
#include <core/a3Record.h>

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

t3Matrix4x4 a3Shape::getShadeToWorld(const a3IntersectRecord& its) const
{
    t3Matrix4x4 shadeToWorld(shapeToWorld._mat[0][0], shapeToWorld._mat[0][1], shapeToWorld._mat[0][2], 0,
                             shapeToWorld._mat[1][0], shapeToWorld._mat[1][1], shapeToWorld._mat[1][2], 0,
                             shapeToWorld._mat[2][0], shapeToWorld._mat[2][1], shapeToWorld._mat[2][2], 0,
                             0, 0, 0, 1);
    return shadeToWorld;
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
