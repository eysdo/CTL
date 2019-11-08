#include "CTL_queue.h"

#include "CTL_queue.c"

int main(void)
{
    CTL_queue data;
    CTL_queue_new(&data);
    CTL_queue_push(&data, 0x1);
    void* t;
    CTL_queue_pop(&data, &t);
    CTL_queue_push(&data, 0x8);
    CTL_queue_push(&data, 0x9);
    return 0;
}