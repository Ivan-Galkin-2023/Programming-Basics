#include "Parallelogram.h"

#include <math.h>

float area(Parallelogram p)
{
    return p.width * p.height * (float)sin((p.angle * PI) / 180);
}

Parallelogram create_parallelogram(int x, int y, float width, float height, float angle)
{
    Parallelogram p = {x, y, angle, width, height};
    return p;
}

float perimeter(Parallelogram p)
{
    return (p.width + p.height) * 2;
}
