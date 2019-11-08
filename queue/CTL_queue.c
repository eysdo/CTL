#include <malloc.h>
#include <stdint.h>

#include "CTL_queue.h"

int CTL_queue_new(CTL_queue *handle)
{
    handle->size = 0;
    handle->queue.head = (__CTL_QueueNode*)malloc(sizeof(__CTL_QueueNode));
    handle->queue.tail = handle->queue.head;
    handle->queue.tail->next = NULL;
    return 0;
}

int CTL_queue_push(CTL_queue *handle, void *data)
{
    __CTL_QueueNode* new_node = (__CTL_QueueNode*)malloc(sizeof(__CTL_QueueNode));
	if (!new_node)
		return -1;

	new_node->data = data;
	new_node->next = NULL;
	handle->queue.tail->next = new_node;
	handle->queue.tail = new_node;
    ++handle->size;
	return 0;
}

int CTL_queue_pop(CTL_queue *handle, void **data)
{
    if (handle->queue.head == handle->queue.tail)
        return -1;

    __CTL_QueueNode *old_node = handle->queue.head->next;
    *data = old_node->data;
    handle->queue.head->next = old_node->next;
    --handle->size;

    if (handle->queue.tail == old_node)
        handle->queue.tail = handle->queue.head;

    free(old_node);
    return 0;
}

int CTL_queue_delete(CTL_queue *handle)
{
    for (__CTL_QueueNode *node = handle->queue.head; node; )
	{
		__CTL_QueueNode *old_node = node->next;
		free(node);
		node = old_node;
	}
    
    return 0;
}

int CTL_queue_front(CTL_queue *handle, void **data)
{
    if (handle->queue.head == handle->queue.tail)
        return -1;

    *data = handle->queue.head->next->data;
    return 0;
}

int CTL_queue_back(CTL_queue *handle, void **data)
{
    if (handle->queue.head == handle->queue.tail)
        return -1;

    *data = handle->queue.tail->data;
}