#include <accelerators/a3PrimitiveSet.h>
#include <shapes/a3Shape.h>
#include <core/log/a3Log.h>

a3PrimitiveSet::~a3PrimitiveSet()
{

}

bool a3PrimitiveSet::addShape(a3Shape* shape)
{
    if(!shape)
    {
        a3Log::error("a3PrimitiveSet::addShape() 空的形状对象指针%d\n", shape);
        return false;
    }

    primitives.push_back(shape);

    return true;
}

