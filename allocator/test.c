#include "CTL_allocator.c"

#include <stdio.h>

#define Judge_Char(var) _Generic((var), \
                    const char:Is_char,  \
                    default:Not_char)

#define Is_char printf("Is char\n")
#define Not_char printf("Not char\n")

#define TEST1(var) 9 + var
#define TEST2(var) 8 + var

int main(void)
{
    const char c;
    printf("%d\n", TEST2(TEST2(10)));
    Judge_Char(c);
    return 0;
}