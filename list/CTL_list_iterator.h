#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#include "CTL_list.h"

typedef struct
{
    __CTL_DuLNode *node;
    uint64_t pos;
}CTL_list_iterator;

int CTL_list_iterator_add(CTL_list_iterator *handle, uint64_t pos);

int CTL_list_iterator_sub(CTL_list_iterator *handle, uint64_t pos);