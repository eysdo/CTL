#include "CTL_queue.h"
#include "../vector/CTL_vector.h"

CTL_VECTOR(int);
CTL_QUEUE(CTL_vector, int);

int main(void)
{
    CTL_vector_int container;
    CTL_vector_new_int(&container, 10);
    CTL_vector_queue_int data;
    CTL_vector_queue_new_int(&data, &container);
    CTL_vector_queue_push_int(&data, 0x1);
    int value;
    CTL_vector_queue_pop_int(&data, &value);
    CTL_vector_queue_push_int(&data, 0x8);
    CTL_vector_queue_push_int(&data, 0x9);
    return 0;
}