#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(int argc, char* argv[])
{
    int a, b;
    if (argc < 3)
    {
        printf("Argument count exception.");
        return -1;
    }
    freopen("output.txt", "w", stdout);
    sscanf_s(argv[1], "%d", &a);
    sscanf_s(argv[2], "%d", &b);
    printf_s("%d", a + b);

    return 0;
}
