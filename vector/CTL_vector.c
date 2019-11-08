#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "CTL_vector.h"
#include "CTL_vector_iterator.h"

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

int CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, void *data)
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

    for (uint64_t i = handle->size - 1; i > iterator.pos; --i)
    {
        handle->base[i] = handle->base[i - 1];
    }
    handle->base[iterator.pos - 1] = data;

    return 0;
}

int CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator)
{
    for (uint64_t i = iterator.pos - 1; i < handle->size; ++i)
    {
        handle->base[i] = handle->base[i + 1];
    }
    --handle->size;
    return 0;
}

int CTL_vector_delete(CTL_vector *handle)
{
    free(handle->base);
    return 0;
}

int CTL_vector_take_iterator(CTL_vector *handle, CTL_vector_iterator *iterator, uint64_t pos)
{
    if(pos > handle->size)
        return -1;
    
    iterator->data = handle->base + pos - 1;
    iterator->pos = pos;
    return 0;
}