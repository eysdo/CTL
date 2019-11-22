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
    ptrdiff_t pos;
} CTL_vector_iterator;

static inline void CTL_vector_new(CTL_vector *handle, size_t size);
static inline void CTL_vector_delete(CTL_vector *handle);
static inline void CTL_vector_clear(CTL_vector *handle);

static inline void CTL_vector_push_front(CTL_vector *handle, type data);
static inline void CTL_vector_pop_front(CTL_vector *handle);

static inline void CTL_vector_push_back(CTL_vector *handle, type data);
static inline void CTL_vector_pop_back(CTL_vector *handle);

static inline void CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, type data);
static inline void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator);

static inline void CTL_vector_at(CTL_vector *handle, CTL_vector_iterator *iterator, size_t pos);
static inline void CTL_vector_iterator_add(CTL_vector_iterator *handle, size_t pos);
static inline void CTL_vector_iterator_sub(CTL_vector_iterator *handle, size_t pos);

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

static inline void CTL_vector_push_front(CTL_vector *handle, type data)
{
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
        memmove(ptr + 1, handle->base, handle->capacity);
        deallocate(handle->base, handle->capacity * sizeof(type));
        handle->base = ptr;
        handle->capacity *= 2;
    }
    else
    {
        memmove(handle->base + 1, handle->base, handle->size);
    }

    handle->base[0] = data;
    ++handle->size;
}

static inline void CTL_vector_pop_front(CTL_vector *handle)
{
    memmove(handle->base, handle->base+1, handle->size);
    --handle->size;
}

static inline void CTL_vector_push_back(CTL_vector *handle, type data)
{
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
        memcpy(ptr, handle->base, handle->capacity);
        deallocate(handle->base, handle->capacity * sizeof(type));
        handle->base = ptr;
        handle->capacity *= 2;
    }

    handle->base[handle->size] = data;
    ++handle->size;
}

static inline void CTL_vector_pop_back(CTL_vector *handle)
{
    --handle->size;
}

static inline void CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, type data)
{
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
        memcpy(ptr, handle->base, sizeof(type) * iterator.pos);
        memcpy(ptr + iterator.pos + 1, handle->base + iterator.pos, sizeof(type) * (handle->size - iterator.pos));
        deallocate(handle->base, handle->capacity * sizeof(type));
        handle->base = ptr;
        handle->capacity *= 2;
    }
    else
    {
         memmove(iterator.data + 1, iterator.data, sizeof(type) * (handle->size - iterator.pos));
    }

    handle->base[iterator.pos] = data;
    ++handle->size;
}

static inline void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator)
{
    memmove(iterator.data, iterator.data + 1, sizeof(type) * (handle->size - iterator.pos - 1));
    --handle->size;
}

static inline void CTL_vector_at(CTL_vector *handle, CTL_vector_iterator *iterator, size_t pos)
{
    iterator->data = handle->base + pos;
    iterator->pos = pos;
}

static inline void CTL_vector_iterator_add(CTL_vector_iterator *handle, size_t pos)
{
    handle->data += pos;
    handle->pos += pos;
}

static inline void CTL_vector_iterator_sub(CTL_vector_iterator *handle, size_t pos)
{
    handle->data -= pos;
    handle->pos -= pos;
}
