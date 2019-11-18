#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "../error/CTL_error.h"

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
    size_t pos;
    size_t size;
} CTL_vector_iterator;

static inline int CTL_vector_new(CTL_vector *handle, size_t size);
static inline void CTL_vector_delete(CTL_vector *handle);
static inline void CTL_vector_clear(CTL_vector *handle);

static inline int CTL_vector_push_front(CTL_vector *handle, type data);
static inline void CTL_vector_pop_front(CTL_vector *handle);

static inline int CTL_vector_push_back(CTL_vector *handle, type data);
static inline void CTL_vector_pop_back(CTL_vector *handle);

static inline int CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, type data);
static inline void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator);

static inline int CTL_vector_at(CTL_vector *handle, CTL_vector_iterator *iterator, size_t pos);
static inline int CTL_vector_iterator_add(CTL_vector_iterator *handle, size_t pos);
static inline int CTL_vector_iterator_sub(CTL_vector_iterator *handle, size_t pos);

/*实现*/
static inline int CTL_vector_new(CTL_vector *handle, size_t size)
{
    handle->size = 0;
    handle->capacity = size;
    handle->base = (type *)malloc(sizeof(type) * size);

    if (!handle->base)
        return CTL_MALLOC_FAILED;
    return 0;
}

static inline void CTL_vector_delete(CTL_vector *handle)
{
    CTL_vector_clear(handle);
    free(handle->base);
}

static inline void CTL_vector_clear(CTL_vector *handle)
{
    handle->size = 0;
}

static inline int CTL_vector_push_front(CTL_vector *handle, type data)
{
    if (handle->size > handle->capacity)
    {
        type *ptr = (type *)malloc(2 * handle->capacity * sizeof(type));
        if (!ptr)
            return CTL_MALLOC_FAILED;
        memcpy(ptr + 1, handle->base, handle->capacity);
        free(handle->base);
        handle->base = ptr;
    }
    else
        memmove(handle->base + 1, handle->base, handle->size);
    
    handle->base[0] = data;

    ++handle->size;
    return 0;
}

static inline void CTL_vector_pop_front(CTL_vector *handle)
{
    memmove(handle->base, handle->base+1, handle->size);
    --handle->size;
}

static inline int CTL_vector_push_back(CTL_vector *handle, type data)
{

    if (handle->size > handle->capacity)
    {
        type *ptr = (type *)malloc(2 * handle->capacity * sizeof(type));
        if (!ptr)
            return CTL_MALLOC_FAILED;
        memcpy(ptr, handle->base, handle->capacity);
        free(handle->base);
        handle->base = ptr;
    }

    handle->base[handle->size] = data;
    ++handle->size;
    return 0;
}

static inline void CTL_vector_pop_back(CTL_vector *handle)
{
    --handle->size;
}

static inline int CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, type data)
{
    if (handle->size >= handle->capacity)
    {
        type *ptr = (type *)malloc(2 * handle->capacity * sizeof(type));
        if (!ptr)
            return CTL_MALLOC_FAILED;
        handle->capacity *= 2;
        memcpy(ptr, handle->base, sizeof(type) * iterator.pos);
        memcpy(ptr + iterator.pos + 1, handle->base + iterator.pos, sizeof(type) * (handle->size - iterator.pos));
        free(handle->base);
        handle->base = ptr;
    }
    else
    {
         memmove(iterator.data + 1, iterator.data, sizeof(type) * (handle->size - iterator.pos));
    }

    handle->base[iterator.pos] = data;

    ++handle->size;
    return 0;
}

static inline void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator)
{
    memmove(iterator.data, iterator.data + 1, sizeof(type) * (handle->size - iterator.pos - 1));
    --handle->size;
}

static inline int CTL_vector_at(CTL_vector *handle, CTL_vector_iterator *iterator, size_t pos)
{
    if (pos > handle->size - 1 || pos < 0)
        return CTL_OUT_OF_RANGE;
    iterator->size = handle->size;
    iterator->data = handle->base + pos;
    iterator->pos = pos;
    return 0;
}

static inline int CTL_vector_iterator_add(CTL_vector_iterator *handle, size_t pos)
{
    if (handle->pos + pos > handle->size - 1)
        return CTL_OUT_OF_RANGE;
    handle->data += pos;
    handle->pos += pos;
    return 0;
}

static inline int CTL_vector_iterator_sub(CTL_vector_iterator *handle, size_t pos)
{
    if (handle->pos - pos < 0)
        return CTL_OUT_OF_RANGE;
    handle->data -= pos;
    handle->pos -= pos;
    return 0;
}
