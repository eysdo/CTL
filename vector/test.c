#include "CTL_vector.h"
#include "../allocator/CTL_allocator.c"

int main(void)
{
//    CTL_vector_queue;
    CTL_vector data;
    CTL_vector_new(&data, 1);
    CTL_vector_push_back(&data, 1);
    CTL_vector_push_front(&data, 0x8);
    CTL_vector_push_back(&data, 0x9);
    CTL_vector_iterator it;
    CTL_vector_at(&data, &it, 0);
    //CTL_vector_iterator_sub(&it, 1);
    CTL_vector_insert(&data, it, 99);
    CTL_vector_at(&data, &it, 0);
    CTL_vector_erase(&data, it);
    CTL_vector_delete(&data);
    printf("%d\n", Memory);
    return 0;
}