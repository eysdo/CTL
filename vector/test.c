#include "CTL_vector.h"

CTL_VECTOR(int);

#define CTL_QUEUE(type_container, type)  \
    typedef struct                       \
    {                                    \
        type_container##_##type t; \
    } type_container##_##queue_##type;

//CTL_QUEUE(CTL_vector, int);

int main(void)
{
//    CTL_vector_queue_int;
    CTL_vector_int data;
    CTL_vector_new_int(&data, 10);
    CTL_vector_push_back_int(&data, 0x1);
    CTL_vector_pop_front_int(&data);
    CTL_vector_push_front_int(&data, 0x8);
    CTL_vector_push_back_int(&data, 0x9);
    CTL_vector_pop_front_int(&data);
    CTL_vector_iterator_int it;
    CTL_vector_at_int(&data, &it, 0);
    //CTL_vector_iterator_sub_int(&it, 1);
    CTL_vector_insert_int(&data, it, 99);
    return 0;
}