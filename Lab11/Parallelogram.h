#ifndef H_PARALLELOGRAM
#define H_PARALLELOGRAM

#define PI 3.1415926535

typedef struct parallelogram
{
    int x;
    int y;
    float angle;
    float width;
    float height;
} Parallelogram;

float perimeter(Parallelogram p);
float area(Parallelogram p);
Parallelogram create_parallelogram(int x, int y, float width, float height, float angle);

#endif