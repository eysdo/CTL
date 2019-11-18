#include "CTL_list.h"

int main(void)
{
    CTL_list data;
    CTL_list_new(&data);
    CTL_list_push_back(&data, 1);
    CTL_list_push_back(&data, 2);
    CTL_list_push_back(&data, 3);
    CTL_list_push_back(&data, 4);
    CTL_list_push_back(&data, 5);
    CTL_list_delete(&data);
    CTL_list_push_back(&data, 6);
    CTL_list_push_back(&data, 7);
    CTL_list_push_back(&data, 8);
    CTL_list_push_back(&data, 9);
    CTL_list_push_back(&data, 10);

    CTL_list_push_back(&data, 0x1);
    CTL_list_pop_front(&data);
    CTL_list_push_front(&data, 0x8);
    CTL_list_push_back(&data, 0x9);
    CTL_list_pop_front(&data);

    CTL_list_iterator it;
    CTL_list_at(&data, &it, 5);
    *it.data = 999;
    return 0;
}