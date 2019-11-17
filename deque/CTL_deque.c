
#include "../error/CTL_error.h"

#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define __CTL_DEQUE_NEW_NODE(node, size)              \
    do                                                \
    {                                                 \
        int *buf = (int *)malloc(sizeof(int) * size); \
        if (!buf)                                     \
            return CTL_MALLOC_FAILED;                 \
        node.base = buf;                              \
        node.border = &buf[size - 1];                 \
        node.first = NULL;                            \
        node.last = NULL;                             \
    } while (0);

//#define __CTL_DEQUE_MAP_EXPANSION

typedef struct CTL_deque_node
{
    int *base;  //基地址
    int *first; //第一个存储数据的地址
    int *last;  //最后一个存储数据的地址
    //uint64_t capacity;
} CTL_deque_node;

typedef struct
{
    int *data;
    CTL_deque_node *node;
    uint64_t pos;
    uint64_t size;
} CTL_deque_iterator;

typedef struct
{
    struct
    {
        CTL_deque_node *base;
        uint64_t size;
        uint64_t capacity;
    } map;
    uint64_t buf_size;
    uint64_t size;
    //uint64_t capacity;
    CTL_deque_node *start;  //第一个指针
    CTL_deque_node *finish; //最后一个指针
} CTL_deque;

int __CTL_DEQUE_MAP_EXPANSION(CTL_deque *handle)
{

    uint64_t lave = handle->map.capacity - handle->map.size;
    CTL_deque_node *start;
    CTL_deque_node *finish;

    if (lave > 4)
    {
        start = handle->map.base + (lave / 2);
        finish = start + handle->map.size - 1;

        memmove(start, handle->start, sizeof(CTL_deque_node) * handle->map.size);
    }
    else
    {
        uint64_t size = handle->map.capacity * 2;
        CTL_deque_node *base = (CTL_deque_node *)malloc(sizeof(CTL_deque_node) * size);
        if (!base)
            return CTL_MALLOC_FAILED;

        lave = size - handle->map.size;
        start = base + (lave / 2);
        finish = start + handle->map.size - 1;

        memcpy(start, handle->start, sizeof(CTL_deque_node) * handle->map.size);

        free(handle->map.base);
        handle->map.base = base;
        handle->map.capacity = size;
    }
    handle->start = start;
    handle->finish = finish;
    return 0;
}

int CTL_deque_new(CTL_deque *handle, uint64_t size)
{
    handle->map.capacity = size;
    handle->buf_size = size;
    handle->map.base = (CTL_deque_node *)malloc(sizeof(CTL_deque_node) * size);

    if (!handle->map.base)
        return CTL_MALLOC_FAILED;
    handle->start = handle->map.base + (size / 2);

    handle->start->base = (int *)malloc(sizeof(int) * handle->buf_size);
    handle->map.size = 1;
    handle->start->first = handle->start->base;
    handle->start->last = handle->start->first - 1;
    handle->finish = handle->start;
    handle->size = 0;
    //handle->capacity = 0;
    return 0;
}

int CTL_deque_push_front(CTL_deque *handle, int data)
{

    if (handle->start->first == handle->start->base)
    {
        __CTL_DEQUE_MAP_EXPANSION(handle);
        --handle->start;

        ++handle->map.size;
        handle->start->base = (int *)malloc(sizeof(int) * handle->buf_size);
        if (!handle->start->base)
            return CTL_MALLOC_FAILED;
        handle->start->first = handle->start->base + handle->buf_size - 1;
        handle->start->last = handle->start->first;
        *handle->start->first = data;
    }
    else
    {
        *--handle->start->first = data;
    }

    ++handle->size;
    return 0;
}

int CTL_deque_pop_front(CTL_deque *handle)
{
    if (handle->size < 1)
        return CTL_OUT_OF_RANGE;

    if (handle->start->first == handle->start->base)
    {
        free(handle->start->base);
        ++handle->start;
        --handle->map.size;
    }
    else
    {
        ++handle->start->first;
    }
    --handle->size;
    return 0;
}

int CTL_deque_push_back(CTL_deque *handle, int data)
{
    if (handle->finish->last == handle->finish->base + handle->buf_size - 1)
    {
        __CTL_DEQUE_MAP_EXPANSION(handle);
        ++handle->finish;
        ++handle->map.size;
        handle->finish->base = (int *)malloc(sizeof(int) * handle->buf_size);
        if (!handle->finish->base)
            return CTL_MALLOC_FAILED;
        handle->finish->first = handle->finish->base;
        handle->finish->last = handle->finish->first;
        *handle->finish->last = data;
    }
    else
    {
        *++handle->finish->last = data;
    }

    ++handle->size;
    return 0;
}

int CTL_deque_pop_back(CTL_deque *handle)
{
    if (handle->size < 1)
        return CTL_OUT_OF_RANGE;

    if (handle->finish->last == handle->finish->base)
    {
        free(handle->finish->base);
        --handle->finish;
        --handle->map.size;
    }
    else
    {
        --handle->finish->last;
    }
    --handle->size;
    return 0;
}

int CTL_deque_insert(CTL_deque *handle, CTL_deque_iterator iterator, int data)
{
    int ret = CTL_deque_push_back(handle, *handle->finish->last);
    if (ret)
        return ret;
    CTL_deque_node *finish = (handle->finish->first == handle->finish->last) ? handle->finish - 1 : handle->finish;
    uint64_t node_pos = iterator.pos / handle->buf_size;

    int *first = finish->first;
    int *last = finish->last;
    for (size_t i = handle->map.size - 1; i > node_pos; --i)
    {

        memmove(first + 1, first, sizeof(int) * (last - first - 1));
        --finish;
        *first = *finish->last;
        first = finish->first;
        last = finish->last;
    }

    if (iterator.data != finish->last)
        memmove(iterator.data + 1, iterator.data, sizeof(int) * (finish->last - iterator.data));

    *iterator.data = data;
}

int CTL_deque_erase(CTL_deque *handle, CTL_deque_iterator iterator)
{
    /*
    int ret = CTL_deque_push_back(handle, *handle->finish->last);
    if (ret)
        return ret;
    CTL_deque_node *finish = (handle->finish->first == handle->finish->last) ? handle->finish - 1 : handle->finish;
    */

    if (iterator.data != iterator.node->last)
    {
        //memmove(iterator.data, iterator.data + 1, sizeof(int) * (iterator.node->last - iterator.data - 1));
        uint64_t node_pos = iterator.pos / handle->buf_size;
        CTL_deque_node *start = &handle->start[node_pos];
        int *first = iterator.data;
        int *last = start->last;
        for (size_t i = handle->map.size - 1; i > node_pos; --i)
        {
            memmove(first, first + 1, sizeof(int) * (last - first));
            ++start;
            *last = *start->first;
            first = start->first;
            last = start->last;
        }
    }
    else
    {
        if(iterator.node == handle->start)
        {
            free(handle->start->base);
            ++handle->start;
            --handle->map.size;
        }
        else if (iterator.node == handle->finish)
        {
           free(handle->finish->base);
           --handle->finish;
           --handle->map.size;
        }
        return 0;
    }
}

int CTL_deque_at(CTL_deque *handle, CTL_deque_iterator *iterator, uint64_t pos)
{
    if (pos > handle->size - 1 || pos < 0)
        return CTL_OUT_OF_RANGE;
    uint64_t data_pos = (pos + (handle->start->first - handle->start->base)) % handle->buf_size;
    uint64_t node_pos = pos / handle->buf_size;

    iterator->pos = pos;
    iterator->size = handle->size;
    iterator->node = &handle->start[node_pos];
    iterator->data = &iterator->node->first[data_pos];
    return 0;
}

int main(void)
{

    CTL_deque handle;
    CTL_deque_new(&handle, 10);
    //CTL_deque_push_front(&handle, 1);

    for (size_t i = 0; i < 100; i++)
    {
        CTL_deque_push_back(&handle, i);
        //printf("%d\n", *handle.finish->last);
    }
    CTL_deque_iterator it;
    CTL_deque_at(&handle, &it, 0);
    CTL_deque_erase(&handle, it);
    //printf("%d \n\n", *it.data);

    for (size_t i = 0; i < 100; i++)
    {
        printf("%d\n", *handle.finish->last);
        CTL_deque_pop_back(&handle);
    }
    //printf("%d\n", *handle.finish->last);

    return 0;
}

/*
int CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator);

int CTL_vector_delete(CTL_vector *handle);

int CTL_vector_take_iterator(CTL_vector *handle, CTL_vector_iterator *iterator, uint64_t pos);
*/