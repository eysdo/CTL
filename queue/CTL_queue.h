#pragma once

#include <malloc.h>
#include <stdint.h>

typedef struct __CTL_QueueNode
{
	void* data;
	struct __CTL_QueueNode* next;
}__CTL_QueueNode;

typedef struct
{
    struct
    {
        __CTL_QueueNode *head;
        __CTL_QueueNode *tail;
        /* data */
    } queue;

    uint64_t size;
} CTL_queue;

int CTL_queue_new(CTL_queue *handle);

int CTL_queue_push(CTL_queue *handle, void *data);

int CTL_queue_pop(CTL_queue *handle, void **data);

int CTL_queue_delete(CTL_queue *handle);

int CTL_queue_front(CTL_queue *handle, void **data);

int CTL_queue_back(CTL_queue *handle, void **data);