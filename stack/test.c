#include "CTL_stack.h"

#include "CTL_stack.c"

int main(void)
{
    CTL_stack data;
    CTL_stack_new(&data);
    CTL_stack_push(&data, 0x1);
    void* t;
    CTL_stack_pop(&data, &t);
    CTL_stack_push(&data, 0x8);
    CTL_stack_push(&data, 0x9);
    return 0;
}