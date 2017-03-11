#include <shapes/a3ShapeSet.h>
#include <shapes/a3Shape.h>
#include <samples/a3LightSample.h>

a3ShapeSet::a3ShapeSet()
{

}

a3ShapeSet::~a3ShapeSet()
{

}

float a3ShapeSet::area() const
{
    return sumArea;
}

t3Vector3f a3ShapeSet::sample(const a3LightSample& sample, t3Vector3f* normal) const
{
    // 随机选择shape
    int selected = shapes.size() * sample.uPos[0];
    a3Shape* shape = shapes[selected];

    t3Vector3f p = shape->sample(sample);

    // 指定点获取法线
    *normal = shape->getNormal(p, sample.uPos[0], sample.uPos[1]);

    return p;
}

float a3ShapeSet::pdf(const t3Vector3f& p, const t3Vector3f& wi) const
{
    float pdf = 0.0f;

    for(auto s : shapes)
        pdf += s->pdf(p, wi);

    // ……不明觉厉
    return pdf / sumArea;
}

bool a3ShapeSet::addShape(a3Shape* shape)
{
    if(shape)
    {
        shapes.push_back(shape);
        sumArea += shape->area();

        return true;
    }

    return false;
}
