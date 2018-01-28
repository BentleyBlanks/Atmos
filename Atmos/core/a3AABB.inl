#pragma once

#include <core/a3AABB.h>

const t3Vector3f& a3AABB::operator[](int i) const
{
    a3Assert(i == 0 || i == 1);
    // --!不明觉厉的写法
    return (&min)[i];
}

t3Vector3f& a3AABB::operator[](int i) 
{
    a3Assert(i == 0 || i == 1);
    // --!不明觉厉的写法
    return (&min)[i];
}

bool a3AABB::operator==(const a3AABB& bbox) const
{
    return bbox.min == min && bbox.max == max;
}

bool a3AABB::operator!=(const a3AABB& bbox) const
{
    return bbox.min != min || bbox.max != max;
}


