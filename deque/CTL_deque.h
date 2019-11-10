
#include "../vector/CTL_vector.h"

typedef struct
{
    void **base;
    uint64_t size;
} __CTL_deque_node;

typedef struct
{
    CTL_vector map;
    uint64_t size;
    uint64_t capacity;
    uint64_t buffer_size;
}CTL_deque;


int CTL_deque_new(CTL_deque *handle, uint64_t size)
{
    handle->capacity = size;
    handle->buffer_size = size;
    CTL_vector_new(&handle->map, size);
    
    __CTL_deque_node* node = (__CTL_deque_node*)malloc(sizeof(__CTL_deque_node));
    void** buf = (void**)malloc(sizeof(void*) * size);
    node->base = buf;
    node->size = 0;
    
    CTL_vector_push(&handle, node);
    return 0;
}

int CTL_deque_head_push(CTL_deque *handle, void *data)
{
    handle->map
}

int CTL_vector_pop(CTL_vector *handle);

int CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator iterator, void *data);

int CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator iterator);

int CTL_vector_delete(CTL_vector *handle);

int CTL_vector_take_iterator(CTL_vector *handle, CTL_vector_iterator *iterator, uint64_t pos);