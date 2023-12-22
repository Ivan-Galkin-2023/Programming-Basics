#define _CRT_SECURE_NO_WARNINGS
#include <string.h>

void clear(char* str, char* current)
{
    if (*current == 0)
        return;
    if (*current == ' ' && *(current + 1) == ' ')
        strcpy(str + *current, current + 1);
    clear(str, current);
}

int main(int argc, char* argv[])
{
    char text[] = " hello   world  ";
    clear(text, text);
    return 0;
}
