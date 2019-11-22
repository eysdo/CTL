#include "CTL_list.h"
#include "../allocator/CTL_allocator.c"


int main(void)
{
    //    CTL_list_queue;
    CTL_list data;
    CTL_list_new(&data);
    CTL_list_push(&data, 1, CTL_NEXT);
    CTL_list_push(&data, 2, 0);
    CTL_list_push(&data, 3, 1);
    //CTL_list_iterator_sub(&it, 1);
    CTL_list_delete(&data);
    printf("%d\n", Memory);
    return 0;
}