#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(int argc, char* argv[])
{
    int a, b;
    freopen("output.txt", "w", stdout);
    sscanf_s(argv[1], "%d", &a);
    sscanf_s(argv[2], "%d", &b);
    printf_s("%d", a + b);

    return 0;
}
