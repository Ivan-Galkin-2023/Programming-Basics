#include <math.h>
#include <stdio.h>
#include "Parallelogram.h"

int main(int argc, char* argv[])
{
    int x;
    int y;
    float angle;
    float width;
    float height;
    printf("X: ");
    scanf_s("%d", &x);
    printf("Y: ");
    scanf_s("%d", &y);
    printf("Width: ");
    scanf_s("%f", &width);
    printf("Height: ");
    scanf_s("%f", &height);
    printf("Angle: ");
    scanf_s("%f", &angle);

    Parallelogram p = create_parallelogram(x, y, width, height, angle);
    printf("Area = %f\n", area(p));
    printf("Perimeter = %f", perimeter(p));
    return 0;
}
