#pragma once

#include <malloc.h>
#include <stdint.h>

typedef struct __CTL_DuLNode
{
	void *data;
	struct __CTL_DuLNode *prior;
	struct __CTL_DuLNode *next;
} __CTL_DuLNode;

#include "CTL_list_iterator.h"

typedef struct
{
	struct
	{
		__CTL_DuLNode head;
		__CTL_DuLNode *tail;
	} list;

	uint64_t size;
} CTL_list;

int CTL_list_new(CTL_list *handle);

int CTL_list_push(CTL_list *handle, void *data);

int CTL_list_pop(CTL_list *handle);

int CTL_list_insert(CTL_list *handle, CTL_list_iterator iterator, void *data);

int CTL_list_erase(CTL_list *handle, CTL_list_iterator iterator);

int CTL_list_delete(CTL_list *handle);

int CTL_list_take_iterator(CTL_list *handle, CTL_list_iterator *iterator, uint64_t pos);