#include "CTL_list.h"

CTL_LIST(int);

int main(void)
{
    CTL_list_int data;
    CTL_list_new_int(&data);
    CTL_list_push_back_int(&data, 0x1);
    CTL_list_pop_front_int(&data);
    CTL_list_push_front_int(&data, 0x8);
    CTL_list_push_back_int(&data, 0x9);
    //CTL_list_pop_front_int(&data);
    CTL_list_iterator_int it;
    CTL_list_at_int(&data, &it, 1);
    CTL_list_iterator_sub_int(&it, 1);
    return 0;
}