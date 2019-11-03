#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#define CTL_vector(type, name)                                                              \
    typedef struct                                                                          \
    {                                                                                       \
        type *data;                                                                         \
        uint64_t size;                                                                      \
        uint64_t capacity;                                                                  \
    } CTL_vector_##name;                                                                    \
                                                                                            \
    static int CTL_vector_new_##name(CTL_vector_##name *handle, uint64_t size)              \
    {                                                                                       \
        handle->size = 0;                                                                   \
        handle->capacity = size;                                                            \
        handle->data = (type *)malloc(sizeof(type) * size);                                 \
                                                                                            \
        if (!handle->data)                                                                  \
            return -1;                                                                      \
                                                                                            \
        return 0;                                                                           \
    }                                                                                       \
                                                                                            \
    static int CTL_vector_push_##name(CTL_vector_##name *handle, type data)                 \
    {                                                                                       \
        ++handle->size;                                                                     \
                                                                                            \
        if (handle->size > handle->capacity)                                                \
        {                                                                                   \
            type *ptr = (type *)malloc(2 * handle->capacity);                               \
            if (!ptr)                                                                       \
                return -1;                                                                  \
            memcpy(ptr, handle->data, handle->capacity);                                    \
            handle->data = ptr;                                                             \
        }                                                                                   \
                                                                                            \
        handle->data[handle->size - 1] = data;                                              \
        return 0;                                                                           \
    }                                                                                       \
                                                                                            \
    static int CTL_vector_pop_##name(CTL_vector_##name *handle)                             \
    {                                                                                       \
        --handle->size;                                                                     \
    }                                                                                       \
                                                                                            \
    static int CTL_vector_insert_##name(CTL_vector_##name *handle, type data, uint64_t pos) \
    {                                                                                       \
        ++handle->size;                                                                     \
        if (handle->size > handle->capacity)                                                \
        {                                                                                   \
            type *ptr = (type *)malloc(2 * handle->capacity);                               \
            if (!ptr)                                                                       \
                return -1;                                                                  \
            memcpy(ptr, handle->data, handle->capacity);                                    \
            handle->data = ptr;                                                             \
        }                                                                                   \
                                                                                            \
        for (uint64_t i = handle->size - 1; i > pos; --i)                                   \
        {                                                                                   \
            handle->data[i] = handle->data[i - 1];                                          \
        }                                                                                   \
        handle->data[pos - 1] = data;                                                       \
                                                                                            \
        return 0;                                                                           \
    }                                                                                       \
                                                                                            \
    static void CTL_vector_erase_##name(CTL_vector_##name *handle, uint64_t pos)            \
    {                                                                                       \
        for (uint64_t i = pos - 1; i < handle->size; ++i)                                   \
        {                                                                                   \
            handle->data[i] = handle->data[i + 1];                                          \
        }                                                                                   \
        --handle->size;                                                                     \
    }                                                                                       \
                                                                                            \
    static void CTL_vector_delete_##name(CTL_vector_##name *handle)                         \
    {                                                                                       \
        free(handle->data);                                                                 \
    }
