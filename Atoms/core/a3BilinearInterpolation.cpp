#include <core/a3BilinearInterpolation.h>

float a3BilinearInterpolation(float x, float y, float f00, float f01, float f11, float f10)
{
    return f00 * (1 - x) * (1 - y) +
           f10 * x * (1 - y) +
           f11 * x * y +
           f01 * (1 - x) * y;
}

