#include "CTL_vector.h"

#include "CTL_vector_iterator.c"
#include "CTL_vector.c"
int main(void)
{
    CTL_vector data;
    CTL_vector_new(&data, 10);
    CTL_vector_push(&data, 0x1);
    CTL_vector_pop(&data);
    CTL_vector_push(&data, 0x8);
    CTL_vector_push(&data, 0x9);
    CTL_vector_iterator it;
    CTL_vector_take_iterator(&data, &it, 2);
    CTL_vector_iterator_sub(&it, 1);
    return 0;
}