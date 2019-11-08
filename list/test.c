#include "CTL_list.h"

#include "CTL_list.c"
#include "CTL_list_iterator.c"

int main(void)
{
    CTL_list data;
    CTL_list_new(&data);
    CTL_list_push(&data, 0x1);
    CTL_list_pop(&data);
    CTL_list_push(&data, 0x8);
    CTL_list_push(&data, 0x9);
    CTL_list_iterator it;
    CTL_list_take_iterator(&data, &it, 2);
    CTL_list_iterator_sub(&it, 1);
    return 0;
}