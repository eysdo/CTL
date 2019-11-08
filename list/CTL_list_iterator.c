#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#include "CTL_list.h"
#include "CTL_list_iterator.h"

int CTL_list_iterator_add(CTL_list_iterator *handle, uint64_t pos)
{
    __CTL_DuLNode *node = handle->node;
    for (uint64_t i = 0; node && i < pos; ++i, node = node->next)
        ;

    CTL_list_iterator ret;
    handle->pos = pos;
    handle->node = node;
    return 0;
}

int CTL_list_iterator_sub(CTL_list_iterator *handle, uint64_t pos)
{
    __CTL_DuLNode *node = handle->node;
    for (uint64_t i = 0; node && i < pos; ++i, node = node->prior)
        ;

    CTL_list_iterator ret;
    handle->pos = pos;
    handle->node = node;
    return 0;
}
