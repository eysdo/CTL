#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "../error/CTL_error.h"
#include "../allocator/CTL_allocator.h"

typedef int type;

typedef struct
{
    type *base;
    size_t size;
    size_t capacity;
} CTL_vector;

typedef struct
{
    type *data;
    type *node;
} CTL_vector_iterator;

static inline void CTL_vector_new(CTL_vector *handle, size_t size);
static inline void CTL_vector_delete(CTL_vector *handle);
static inline void CTL_vector_clear(CTL_vector *handle);

static inline void CTL_vector_push(CTL_vector *handle, type data, int direction);
static inline void CTL_vector_pop(CTL_vector *handle, int direction);

static inline void CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, type data);
static inline void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator);

static inline void CTL_vector_at(CTL_vector *handle, CTL_vector_iterator *iterator, size_t pos);
static inline void CTL_vector_iterator_move(CTL_vector_iterator *handle, size_t pos, int direction);

/*实现*/
static inline void CTL_vector_new(CTL_vector *handle, size_t size)
{
    handle->size = 0;
    handle->capacity = size;
    handle->base = (type *)allocate(sizeof(type) * size);
}

static inline void CTL_vector_delete(CTL_vector *handle)
{
    CTL_vector_clear(handle);
    deallocate(handle->base, sizeof(type) * handle->capacity);
}

static inline void CTL_vector_clear(CTL_vector *handle)
{
    handle->size = 0;
}

static inline void CTL_vector_push(CTL_vector *handle, type data, int direction)
{
    if (direction == CTL_NEXT)
    {
        if (handle->size == handle->capacity)
        {
            type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
            memmove(ptr, handle->base, handle->capacity * sizeof(type));
            deallocate(handle->base, handle->capacity * sizeof(type));
            handle->base = ptr;
            handle->capacity *= 2;
        }

        handle->base[handle->size] = data;
        ++handle->size;
    }
    else
    {
        if (handle->size == handle->capacity)
        {
            type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
            memmove(ptr + 1, handle->base, handle->capacity * sizeof(type));
            deallocate(handle->base, handle->capacity * sizeof(type));
            handle->base = ptr;
            handle->capacity *= 2;
        }
        else
        {
            memmove(handle->base + 1, handle->base, handle->size * sizeof(type));
        }

        handle->base[0] = data;
        ++handle->size;
    }
}

static inline void CTL_vector_pop(CTL_vector *handle, int direction)
{
    if (direction == CTL_NEXT)
    {
        --handle->size;
    }
    else
    {
        memmove(handle->base, handle->base + 1, handle->size * sizeof(type));
        --handle->size;
    }
}

static inline void CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, type data)
{
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
        memmove(ptr, handle->base, sizeof(type) * (iterator.data - handle->base));
        memmove(ptr + (iterator.data - handle->base) + 1, iterator.data, sizeof(type) * (handle->size - (iterator.data - handle->base)));
        deallocate(handle->base, handle->capacity * sizeof(type));
        handle->base = ptr;
        handle->capacity *= 2;
    }
    else
    {
        memmove(iterator.data + 1, iterator.data, sizeof(type) * (handle->size - (iterator.data - handle->base)));
    }

    handle->base[(iterator.data - handle->base)] = data;
    ++handle->size;
}

static inline void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator)
{
    memmove(iterator.data, iterator.data + 1, sizeof(type) * (handle->size - (iterator.data - handle->base) - 1));
    --handle->size;
}

static inline void CTL_vector_at(CTL_vector *handle, CTL_vector_iterator *iterator, size_t pos)
{
    iterator->data = handle->base + pos;
    iterator->node = handle->base + pos;
}

static inline void CTL_vector_iterator_move(CTL_vector_iterator *handle, size_t pos, int direction)
{
    if (direction == CTL_NEXT)
    {
        handle->data += pos;
        handle->node += pos;
    }
    else
    {
        handle->data -= pos;
        handle->node -= pos;
    }
}