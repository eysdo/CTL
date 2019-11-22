#pragma once

#include <malloc.h>
#include <stdint.h>
#include <assert.h>

#include "../error/CTL_error.h"
#include "../allocator/CTL_allocator.h"


typedef int type;

	/*声明*/
typedef struct __CTL_DuLNode
{
	type data;
	struct __CTL_DuLNode *prior;
	struct __CTL_DuLNode *next;
} __CTL_DuLNode;

typedef struct
{
	__CTL_DuLNode *list;

	size_t size;
} CTL_list;

typedef struct
{
	type *data;
	__CTL_DuLNode *node;
} CTL_list_iterator;

static inline void CTL_list_new(CTL_list *handle);
static inline void CTL_list_delete(CTL_list *handle);
static inline void CTL_vector_clear(CTL_list *handle);

static inline void CTL_list_push(CTL_list *handle, type data, int direction);
static inline void CTL_list_pop(CTL_list *handle, int direction);

static inline void CTL_list_insert(CTL_list *handle, CTL_list_iterator iterator, type data);
void CTL_list_erase(CTL_list *handle, CTL_list_iterator iterator);

CTL_list_iterator CTL_list_at(CTL_list *handle, size_t pos);
static inline void CTL_list_iterator_move(CTL_list_iterator *handle, size_t pos, int direction);

/*实现*/
static inline void CTL_list_new(CTL_list *handle)
{
	//构造 一个 头结点
	__CTL_DuLNode *node = (__CTL_DuLNode *)allocate(sizeof(__CTL_DuLNode));
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;
	handle->size = 0;
}

static inline void CTL_list_delete(CTL_list *handle)
{
	CTL_vector_clear(handle);
	deallocate(handle->list, sizeof(__CTL_DuLNode));
}

static inline void CTL_vector_clear(CTL_list *handle)
{
	__CTL_DuLNode *node;
	for (node = handle->list->next; node != handle->list;)
	{
		__CTL_DuLNode *next_node = node->next;
		deallocate(handle->list, sizeof(__CTL_DuLNode));
		node = next_node;
	}

	handle->size = 0;
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;
}

static inline void CTL_list_push(CTL_list *handle, type data, int direction)
{
	if (direction == CTL_NEXT)
	{
		CTL_list_insert(handle, CTL_list_at(handle, handle->size), data);
	}
	else
	{
		CTL_list_insert(handle, CTL_list_at(handle, 0), data);
	}
}

static inline void CTL_list_pop(CTL_list *handle, int direction)
{
	if (direction == CTL_NEXT)
	{
		assert(handle->size);
		CTL_list_erase(handle, CTL_list_at(handle, handle->size - 1));
	}
	else
	{
		assert(handle->size);
		CTL_list_erase(handle, CTL_list_at(handle, 0));
	}
}

static inline void CTL_list_insert(CTL_list *handle, CTL_list_iterator iterator, type data)
{
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)allocate(sizeof(__CTL_DuLNode));

	new_node->data = data;
	new_node->next = iterator.node;
	new_node->prior = iterator.node->prior;

	new_node->prior->next = new_node;
	iterator.node->prior = new_node;

	++handle->size;
}

void CTL_list_erase(CTL_list *handle, CTL_list_iterator iterator)
{
	--handle->size;

	iterator.node->prior->next = iterator.node->next;
	iterator.node->next->prior = iterator.node->prior;

	deallocate(iterator.node, sizeof(__CTL_DuLNode));
}

CTL_list_iterator CTL_list_at(CTL_list *handle, size_t pos)
{
		CTL_list_iterator result;
	__CTL_DuLNode *node = handle->list;

	if (pos + 1 < handle->size / 2)
	{
		for (size_t i = 0; i <= pos; ++i, node = node->next)
			;
	}
	else
	{
		for (size_t i = handle->size; i > pos; --i, node = node->prior)
			;
	}

	result.node = node;
	result.data = &node->data;
	return result;
}

static inline void CTL_list_iterator_move(CTL_list_iterator *handle, size_t pos, int direction)
{
	if (direction == CTL_NEXT)
	{
		__CTL_DuLNode *node = handle->node;
		for (size_t i = 0; i < pos; ++i, node = node->next)
			;
		handle->node = node;
		handle->data = &node->data;
	}
	else
	{
		__CTL_DuLNode *node = handle->node;
		for (size_t i = 0; node && i < pos; ++i, node = node->prior)
			;
		handle->node = node;
		handle->data = &node->data;
	}
}