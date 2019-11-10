#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "../error/CTL_error.h"

#define CTL_VECTOR(type)                                                                                                   \
    /*声明*/                                                                                                             \
    typedef struct                                                                                                         \
    {                                                                                                                      \
        type *base;                                                                                                        \
        uint64_t size;                                                                                                     \
        uint64_t capacity;                                                                                                 \
    } CTL_vector_##type;                                                                                                   \
                                                                                                                           \
    typedef struct                                                                                                         \
    {                                                                                                                      \
        type *data;                                                                                                        \
        uint64_t pos;                                                                                                      \
        uint64_t size;                                                                                                     \
    } CTL_vector_iterator_##type;                                                                                          \
                                                                                                                           \
    static inline int CTL_vector_new_##type(CTL_vector_##type *handle, uint64_t size);                                     \
    static inline int CTL_vector_push_front_##type(CTL_vector_##type *handle, type data);                                  \
    static inline void CTL_vector_pop_front_##type(CTL_vector_##type *handle);                                             \
    static inline int CTL_vector_push_back_##type(CTL_vector_##type *handle, type data);                                   \
    static inline void CTL_vector_pop_back_##type(CTL_vector_##type *handle);                                              \
    static inline int CTL_vector_insert_##type(CTL_vector_##type *handle, CTL_vector_iterator_##type iterator, type data); \
    static inline void CTL_vector_erase_##type(CTL_vector_##type *handle, CTL_vector_iterator_##type iterator);            \
    static inline void CTL_vector_delete_##type(CTL_vector_##type *handle);                                                \
                                                                                                                           \
    static inline int CTL_vector_at_##type(CTL_vector_##type *handle, CTL_vector_iterator_##type *iterator, uint64_t pos); \
    static inline int CTL_vector_iterator_add_##type(CTL_vector_iterator_##type *handle, uint64_t pos);                    \
    static inline int CTL_vector_iterator_sub_##type(CTL_vector_iterator_##type *handle, uint64_t pos);                    \
                                                                                                                           \
    /*实现*/                                                                                                             \
    static inline int CTL_vector_new_##type(CTL_vector_##type *handle, uint64_t size)                                      \
    {                                                                                                                      \
        handle->size = 0;                                                                                                  \
        handle->capacity = size;                                                                                           \
        handle->base = (type *)malloc(sizeof(type) * size);                                                                \
                                                                                                                           \
        if (!handle->base)                                                                                                 \
            return CTL_MALLOC_FAILED;                                                                                      \
        return 0;                                                                                                          \
    }                                                                                                                      \
                                                                                                                           \
    static inline int CTL_vector_push_front_##type(CTL_vector_##type *handle, type data)                                   \
    {                                                                                                                      \
        ++handle->size;                                                                                                    \
        if (handle->size > handle->capacity)                                                                               \
        {                                                                                                                  \
            type *ptr = (type *)malloc(2 * handle->capacity * sizeof(type));                                               \
            if (!ptr)                                                                                                      \
                return CTL_MALLOC_FAILED;                                                                                  \
            memcpy(ptr, handle->base, handle->capacity);                                                                   \
            free(handle->base);                                                                                            \
            handle->base = ptr;                                                                                            \
        }                                                                                                                  \
                                                                                                                           \
        for (uint64_t i = handle->size - 1; i > 1; --i)                                                                    \
        {                                                                                                                  \
            handle->base[i] = handle->base[i - 1];                                                                         \
        }                                                                                                                  \
        handle->base[0] = data;                                                                                            \
                                                                                                                           \
        return 0;                                                                                                          \
    }                                                                                                                      \
                                                                                                                           \
    static inline void CTL_vector_pop_front_##type(CTL_vector_##type *handle)                                              \
    {                                                                                                                      \
        for (uint64_t i = 0; i < handle->size; ++i)                                                                        \
        {                                                                                                                  \
            handle->base[i] = handle->base[i + 1];                                                                         \
        }                                                                                                                  \
        --handle->size;                                                                                                    \
        return;                                                                                                            \
    }                                                                                                                      \
                                                                                                                           \
    static inline int CTL_vector_push_back_##type(CTL_vector_##type *handle, type data)                                    \
    {                                                                                                                      \
        ++handle->size;                                                                                                    \
                                                                                                                           \
        if (handle->size > handle->capacity)                                                                               \
        {                                                                                                                  \
            type *ptr = (type *)malloc(2 * handle->capacity * sizeof(type));                                               \
            if (!ptr)                                                                                                      \
                return CTL_MALLOC_FAILED;                                                                                  \
            memcpy(ptr, handle->base, handle->capacity);                                                                   \
            free(handle->base);                                                                                            \
            handle->base = ptr;                                                                                            \
        }                                                                                                                  \
                                                                                                                           \
        handle->base[handle->size - 1] = data;                                                                             \
        return 0;                                                                                                          \
    }                                                                                                                      \
                                                                                                                           \
    static inline void CTL_vector_pop_back_##type(CTL_vector_##type *handle)                                               \
    {                                                                                                                      \
        --handle->size;                                                                                                    \
    }                                                                                                                      \
                                                                                                                           \
    static inline int CTL_vector_insert_##type(CTL_vector_##type *handle, CTL_vector_iterator_##type iterator, type data)  \
    {                                                                                                                      \
        ++handle->size;                                                                                                    \
        if (handle->size > handle->capacity)                                                                               \
        {                                                                                                                  \
            type *ptr = (type *)malloc(2 * handle->capacity * sizeof(type));                                               \
            if (!ptr)                                                                                                      \
                return CTL_MALLOC_FAILED;                                                                                  \
            memcpy(ptr, handle->base, handle->capacity);                                                                   \
            free(handle->base);                                                                                            \
            handle->base = ptr;                                                                                            \
        }                                                                                                                  \
                                                                                                                           \
        for (uint64_t i = handle->size - 1; i > iterator.pos + 1; --i)                                                     \
        {                                                                                                                  \
            handle->base[i] = handle->base[i - 1];                                                                         \
        }                                                                                                                  \
        handle->base[iterator.pos] = data;                                                                                 \
                                                                                                                           \
        return 0;                                                                                                          \
    }                                                                                                                      \
                                                                                                                           \
    static inline void CTL_vector_erase_##type(CTL_vector_##type *handle, CTL_vector_iterator_##type iterator)             \
    {                                                                                                                      \
        for (uint64_t i = iterator.pos; i < handle->size; ++i)                                                             \
        {                                                                                                                  \
            handle->base[i] = handle->base[i + 1];                                                                         \
        }                                                                                                                  \
        --handle->size;                                                                                                    \
        return;                                                                                                            \
    }                                                                                                                      \
                                                                                                                           \
    static inline void CTL_vector_delete_##type(CTL_vector_##type *handle)                                                 \
    {                                                                                                                      \
        free(handle->base);                                                                                                \
        return;                                                                                                            \
    }                                                                                                                      \
                                                                                                                           \
    static inline int CTL_vector_at_##type(CTL_vector_##type *handle, CTL_vector_iterator_##type *iterator, uint64_t pos)  \
    {                                                                                                                      \
        if (pos > handle->size - 1 || pos < 0)                                                                             \
            return CTL_OUT_OF_RANGE;                                                                                       \
        iterator->size = handle->size;                                                                                     \
        iterator->data = handle->base + pos;                                                                               \
        iterator->pos = pos;                                                                                               \
        return 0;                                                                                                          \
    }                                                                                                                      \
                                                                                                                           \
    static inline int CTL_vector_iterator_add_##type(CTL_vector_iterator_##type *handle, uint64_t pos)                     \
    {                                                                                                                      \
        if (handle->pos + pos > handle->size - 1)                                                                          \
            return CTL_OUT_OF_RANGE;                                                                                       \
        handle->data += pos;                                                                                               \
        handle->pos += pos;                                                                                                \
        return 0;                                                                                                          \
    }                                                                                                                      \
                                                                                                                           \
    static inline int CTL_vector_iterator_sub_##type(CTL_vector_iterator_##type *handle, uint64_t pos)                     \
    {                                                                                                                      \
        if (handle->pos - pos < 0)                                                                                         \
            return CTL_OUT_OF_RANGE;                                                                                       \
        handle->data -= pos;                                                                                               \
        handle->pos -= pos;                                                                                                \
        return 0;                                                                                                          \
    }
