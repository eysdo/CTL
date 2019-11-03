#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include "CTL_vector.h"

CTL_vector(int, int);

typedef struct
{
    int *data;
    uint64_t size;
    uint64_t capacity;
} CTL_vector;

int CTL_vector_new(CTL_vector *handle, uint64_t size)
{
    handle->size = 0;
    handle->capacity = size;
    handle->data = (int *)malloc(sizeof(int) * size);

    if (!handle->data)
        return -1;

    return 0;
}

int CTL_vector_push(CTL_vector *handle, int data)
{
    ++handle->size;

    if (handle->size > handle->capacity)
    {
        int *ptr = (int *)malloc(2 * handle->capacity);
        if (!ptr)
            return -1;
        memcpy(ptr, handle->data, handle->capacity);
        handle->data = ptr;
    }

    handle->data[handle->size - 1] = data;
    return 0;
}

int CTL_vector_pop(CTL_vector *handle)
{
    --handle->size;
}

int CTL_vector_insert(CTL_vector *handle, int data, uint64_t pos)
{
    ++handle->size;
    if (handle->size > handle->capacity)
    {
        int *ptr = (int *)malloc(2 * handle->capacity);
        if (!ptr)
            return -1;
        memcpy(ptr, handle->data, handle->capacity);
        handle->data = ptr;
    }

    for (uint64_t i = handle->size - 1; i > pos; --i)
    {
        handle->data[i] = handle->data[i - 1];
    }
    handle->data[pos - 1] = data;

    return 0;
}

void CTL_vector_erase(CTL_vector *handle, uint64_t pos)
{
    for (uint64_t i = pos - 1; i < handle->size; ++i)
    {
        handle->data[i] = handle->data[i + 1];
    }
    --handle->size;
}

void CTL_vector_delete(CTL_vector *handle)
{
    free(handle->data);
}

int main(void)
{
    return 0;
}