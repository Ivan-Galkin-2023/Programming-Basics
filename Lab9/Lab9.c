
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    unsigned int monthCount;
    float percent, startSum;
    printf("Month count: ");
    scanf_s("%d", &monthCount);
    printf("Percent: ");
    scanf_s("%f", &percent);
    printf("Start sum: ");
    scanf_s("%f", &startSum);

    for (size_t i = 0; i < monthCount; ++i)
    {
        startSum += startSum * (percent / 12 / 100);
        printf("Month %d\tBalance: %f\n", i + 1, startSum);
    }

    return 0;
}
