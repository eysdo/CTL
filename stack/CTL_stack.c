#pragma once

#include <malloc.h>
#include <stdint.h>

#include "CTL_stack.h"

int CTL_stack_new(CTL_stack *handle)
{
    handle->top = NULL;
    handle->size = 0;
    return 0;
}

int CTL_stack_push(CTL_stack *handle, void *data)
{
    	__CTL_StackNode* new_node = (__CTL_StackNode*)malloc(sizeof(__CTL_StackNode));
	if (!new_node)
		return -1;

    ++handle->size;
	new_node->data = data;
	new_node->next = handle->top;
	handle->top = new_node;
	return 0;
}

int CTL_stack_pop(CTL_stack *handle, void **data)
{
    if (!handle->top)
        return -1;

    --handle->size;
    *data = handle->top->data;
    __CTL_StackNode *old_node = handle->top;
    handle->top = handle->top->next;
    free(old_node);
    return 0;
}

int CTL_stack_delete(CTL_stack *handle)
{
    while (handle->top)
	{
		__CTL_StackNode *next_node = handle->top->next;
		free(handle->top);
		handle->top->next = next_node;
	}

    return 0;
}

int CTL_stack_top(CTL_stack *handle, void **data)
{
    *data = handle->top->data;
    return 0;
}