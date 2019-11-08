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

int CTL_vector_new(CTL_vector *handle, uint64_t size)
{
    handle->size = 0;
    handle->capacity = size;
    handle->base = (void **)malloc(sizeof(void *) * size);

    if (!handle->base)
        return -1;

    return 0;
}

int CTL_vector_push(CTL_vector *handle, void* data)
{
    ++handle->size;

    if (handle->size > handle->capacity)
    {
        void **ptr = (void **)malloc(2 * handle->capacity);
        if (!ptr)
            return -1;
        memcpy(ptr, handle->base, handle->capacity);
        free(handle->base);
        handle->base = ptr;
    }

    handle->base[handle->size - 1] = data;
    return 0;
}

int CTL_vector_pop(CTL_vector *handle)
{
    --handle->size;
}

int CTL_vector_insert(CTL_vector *handle, void* data, uint64_t pos)
{
    ++handle->size;
    if (handle->size > handle->capacity)
    {
        void **ptr = (void **)malloc(2 * handle->capacity);
        if (!ptr)
            return -1;
        memcpy(ptr, handle->base, handle->capacity);
        free(handle->base);
        handle->base = ptr;
    }

    for (uint64_t i = handle->size - 1; i > pos; --i)
    {
        handle->base[i] = handle->base[i - 1];
    }
    handle->base[pos - 1] = data;

    return 0;
}

void CTL_vector_erase(CTL_vector *handle, uint64_t pos)
{
    for (uint64_t i = pos - 1; i < handle->size; ++i)
    {
        handle->base[i] = handle->base[i + 1];
    }
    --handle->size;
}

void CTL_vector_delete(CTL_vector *handle)
{
    free(handle->base);
}

CTL_vector_iterator CTL_vector_take_iterator(CTL_vector *handle, uint64_t pos)
{
    CTL_vector_iterator ret;
    ret.data = handle->base + pos;
    ret.pos = pos;
    return ret;
}