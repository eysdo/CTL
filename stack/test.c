#include "CTL_stack.h"
#include "../vector/CTL_vector.h"

CTL_VECTOR(int);
CTL_STACK(CTL_vector, int);

int main(void)
{
    CTL_vector_int data;
    CTL_vector_new_int(&data, 10);
    CTL_vector_stack_push_int(&data, 0x1);
    int t;
    CTL_vector_stack_pop_int(&data, &t);
    CTL_vector_stack_push_int(&data, 0x8);
    CTL_vector_stack_push_int(&data, 0x9);
    return 0;
}