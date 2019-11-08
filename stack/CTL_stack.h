#pragma once

#include <malloc.h>
#include <stdint.h>

typedef struct __CTL_StackNode
{
	void* data;
	struct __CTL_StackNode* next;
}__CTL_StackNode;

typedef struct
{
    __CTL_StackNode* top;
    uint64_t size;
} CTL_stack;

int CTL_stack_new(CTL_stack *handle);

int CTL_stack_push(CTL_stack *handle, void *data);

int CTL_stack_pop(CTL_stack *handle, void **data);

int CTL_stack_delete(CTL_stack *handle);

int CTL_stack_top(CTL_stack *handle, void **data);