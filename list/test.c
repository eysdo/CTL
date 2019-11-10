#include "CTL_list.h"

CTL_LIST(int);

int main(void)
{
    CTL_list_int data;
    CTL_list_new_int(&data);
    CTL_list_push_back_int(&data, 1);
    CTL_list_push_back_int(&data, 2);
    CTL_list_push_back_int(&data, 3);
    CTL_list_push_back_int(&data, 4);
    CTL_list_push_back_int(&data, 5);
    CTL_list_push_back_int(&data, 6);
    CTL_list_push_back_int(&data, 7);
    CTL_list_push_back_int(&data, 8);
    CTL_list_push_back_int(&data, 9);
    CTL_list_push_back_int(&data, 10);

    CTL_list_push_back_int(&data, 0x1);
    CTL_list_pop_front_int(&data);
    CTL_list_push_front_int(&data, 0x8);
    CTL_list_push_back_int(&data, 0x9);
    CTL_list_pop_front_int(&data);

    CTL_list_iterator_int it;
    CTL_list_at_int(&data, &it, 8);
    *it.data = 999;
    return 0;
}