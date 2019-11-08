#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "CTL_vector_iterator.h"

typedef struct
{
    void **base;
    uint64_t size;
    uint64_t capacity;
} CTL_vector;

int CTL_vector_new(CTL_vector *handle, uint64_t size);

int CTL_vector_push(CTL_vector *handle, void *data);

int CTL_vector_pop(CTL_vector *handle);

int CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, void *data);

int CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator);

int CTL_vector_delete(CTL_vector *handle);

int CTL_vector_take_iterator(CTL_vector *handle, CTL_vector_iterator *iterator, uint64_t pos);