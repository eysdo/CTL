
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
        node.border = &buf[size - 1];                  \
        node.first = NULL;                            \
        node.last = NULL;                             \
    } while (0);

//#define __CTL_DEQUE_MAP_EXPANSION

typedef struct
{
    int *data;
    uint64_t pos;
} CTL_deque_iterator;

typedef struct CTL_deque_node
{
    int *base; //基地址
    int *first; //第一个存储数据的地址
    int *last; //最后一个存储数据的地址
    //uint64_t capacity;
} CTL_deque_node;

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
    CTL_deque_node *start; //第一个指针
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
    handle->buf_size= size;
    handle->map.base = (CTL_deque_node*)malloc(sizeof(CTL_deque_node) * size);
    if(!handle->map.base)
        return CTL_MALLOC_FAILED;
    handle->start = handle->map.base + (size/2) - 1;

    handle->start->base = (int *)malloc(sizeof(int) * handle->buf_size);
     handle->map.size = 1;
    handle->start->first = handle->start->base;
    handle->start->last = handle->start->first;
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
    if(handle->size < 1)
        return CTL_OUT_OF_RANGE;

    if(handle->start->first == handle->start->base)
    {
        free(handle->start->base);
        ++handle->start;
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
    if(handle->size < 1)
        return CTL_OUT_OF_RANGE;

    if(handle->finish->last == handle->finish->base)
    {
        free(handle->finish->base);
        --handle->finish;
    }
    else
    {
        --handle->finish->last;
    }
    --handle->size;
    return 0;
}

/*
 int CTL_deque_insert(CTL_deque *handle, CTL_deque_iterator iterator, int data)
 {
    CTL_vector_iterator___CTL_deque_node map;
    CTL_vector_at___CTL_deque_node(&handle->map, &map, handle->map.size - 1);
    int ret = CTL_deque_push_back(handle, *map.data->last);
    if(ret)
        return ret;
    uint64_t data_pos = iterator.pos%handle->buffer_size;
    uint64_t node_pos = (iterator.pos - data_pos)/handle->buffer_size;

    CTL_vector_at___CTL_deque_node(&handle->map, &map, node_pos);
    for (size_t i = handle->map.size - 1; i > node_pos; --i)
    {
        
        for (int* c = map.data->last; c >= map.data->first ; --c)
        {
            
        }
        
    }
    
 }

int CTL_deque_at(CTL_deque *handle, CTL_deque_iterator *iterator, uint64_t pos)
{
    if (pos > handle->size - 1 || pos < 0)
        return CTL_OUT_OF_RANGE;
    uint64_t data_pos = pos%handle->buffer_size;
    uint64_t node_pos = (pos - data_pos)/handle->buffer_size;

    CTL_vector_iterator___CTL_deque_node map;
    CTL_vector_at___CTL_deque_node(&handle->map, &map, node_pos);

    iterator->pos = pos;
    iterator->size = handle->size;
    iterator->node = map.data;
    iterator->data = &map.data->first[data_pos];
    return 0;
}
*/
int main(void)
{
    
    CTL_deque handle;
    CTL_deque_new(&handle, 10);
    //CTL_deque_push_front(&handle, 1);

    for (size_t i = 0; i < 200; i++)
    {
        CTL_deque_push_front(&handle, i);
        printf("%d\n", *handle.finish->last);
    }
    printf("\n\n");
    for (size_t i = 0; i < 200; i++)
    {
        printf("%d\n", *handle.finish->last);
        CTL_deque_pop_back(&handle);
    }
    
    return 0;
}




/*
int CTL_deque_pop_front(CTL_vector *handle);

int CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, void *data);

int CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator);

int CTL_vector_delete(CTL_vector *handle);

int CTL_vector_take_iterator(CTL_vector *handle, CTL_vector_iterator *iterator, uint64_t pos);
*/