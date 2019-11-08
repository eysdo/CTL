#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    void **data;
    uint64_t pos;
} CTL_vector_iterator;

int CTL_vector_iterator_add(CTL_vector_iterator *handle, uint64_t pos);

int CTL_vector_iterator_sub(CTL_vector_iterator *handle, uint64_t pos);