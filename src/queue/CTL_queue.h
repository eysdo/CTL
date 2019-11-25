#pragma once

#define CTL_QUEUE(type_container, type)                                                                  \
    /*声明*/                                                                                           \
    static inline int type_container##_queue_push_##type(type_container##_##type *handle, type data);    \
    static inline void type_container##_queue_pop_##type(type_container##_##type *handle, type *data);   \
    static inline void type_container##_queue_front_##type(type_container##_##type *handle, type *data); \
    static inline void type_container##_queue_back_##type(type_container##_##type *handle, type *data);  \
                                                                                                         \
    /*实现*/                                                                                           \
    static inline int type_container##_queue_push_##type(type_container##_##type *handle, type data)     \
    {                                                                                                    \
        return type_container##_push_back_##type(handle, data);                                          \
    }                                                                                                    \
                                                                                                         \
    static inline void type_container##_queue_pop_##type(type_container##_##type *handle, type *data)    \
    {                                                                                                    \
        type_container##_queue_front_##type(handle, data);                                               \
        type_container##_pop_front_##type(handle);                                                       \
        return;                                                                                          \
    }                                                                                                    \
                                                                                                         \
    static inline void type_container##_queue_front_##type(type_container##_##type *handle, type *data)  \
    {                                                                                                    \
        type_container##_iterator_##type iterator;                                                       \
        type_container##_at_##type(handle, &iterator, 0);                                                \
        *data = *iterator.data;                                                                          \
        return;                                                                                          \
    }                                                                                                    \
                                                                                                         \
    static inline void type_container##_queue_back_##type(type_container##_##type *handle, type *data)   \
    {                                                                                                    \
        type_container##_iterator_##type iterator;                                                       \
        type_container##_at_##type(handle, &iterator, handle->size - 1);                                 \
        *data = *iterator.data;                                                                          \
        return;                                                                                          \
    }
