#pragma once

#include <malloc.h>
#include <stdint.h>

#define CTL_QUEUE(type_container, type)                                                                                                  \
    /*声明*/                                                                                                                           \
    typedef struct                                                                                                                       \
    {                                                                                                                                    \
        type_container##_##type *container;                                                                                              \
    } type_container##_##queue_##type;                                                                                                   \
                                                                                                                                         \
    static inline void type_container##_##queue_new_##type(type_container##_##queue_##type *handle, type_container##_##type *container); \
    static inline int type_container##_queue_push_##type(type_container##_##queue_##type *handle, type data);                            \
    static inline void type_container##_queue_pop_##type(type_container##_##queue_##type *handle, type *data);                           \
    static inline void type_container##_queue_delete_##type(type_container##_##queue_##type *handle);                                    \
    static inline void type_container##_queue_front_##type(type_container##_##queue_##type *handle, type *data);                         \
    static inline void type_container##_queue_back_##type(type_container##_##queue_##type *handle, type *data);                          \
                                                                                                                                         \
    /*实现*/                                                                                                                           \
    static inline void type_container##_##queue_new_##type(type_container##_##queue_##type *handle, type_container##_##type *container)  \
    {                                                                                                                                    \
        handle->container = container;                                                                                                   \
        return;                                                                                                                          \
    }                                                                                                                                    \
                                                                                                                                         \
    static inline int type_container##_queue_push_##type(type_container##_##queue_##type *handle, type data)                             \
    {                                                                                                                                    \
        return type_container##_push_back_##type(handle->container, data);                                                               \
    }                                                                                                                                    \
                                                                                                                                         \
    static inline void type_container##_queue_pop_##type(type_container##_##queue_##type *handle, type *data)                            \
    {                                                                                                                                    \
        type_container##_queue_front_##type(handle, data);                                                                               \
        type_container##_pop_front_##type(handle->container);                                                                            \
        return;                                                                                                                          \
    }                                                                                                                                    \
                                                                                                                                         \
    static inline void type_container##_queue_delete_##type(type_container##_##queue_##type *handle)                                     \
    {                                                                                                                                    \
        type_container##_delete_##type(handle->container);                                                                               \
        return;                                                                                                                          \
    }                                                                                                                                    \
                                                                                                                                         \
    static inline void type_container##_queue_front_##type(type_container##_##queue_##type *handle, type *data)                          \
    {                                                                                                                                    \
        type_container##_iterator_##type iterator;                                                                                       \
        type_container##_at_##type(handle->container, &iterator, 0);                                                                     \
        *data = *iterator.data;                                                                                                          \
        return;                                                                                                                          \
    }                                                                                                                                    \
                                                                                                                                         \
    static inline void type_container##_queue_back_##type(type_container##_##queue_##type *handle, type *data)                           \
    {                                                                                                                                    \
        type_container##_iterator_##type iterator;                                                                                       \
        type_container##_at_##type(handle->container, &iterator, handle->container->size - 1);                                           \
        *data = *iterator.data;                                                                                                          \
        return;                                                                                                                          \
    }
