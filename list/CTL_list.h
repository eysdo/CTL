#pragma once

#include <malloc.h>
#include <stdint.h>

#include "../error/CTL_error.h"

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
	struct
	{
		__CTL_DuLNode head;
		__CTL_DuLNode *tail;
	} list;

	size_t size;
} CTL_list;

typedef struct
{
	__CTL_DuLNode *node;
	type *data;
	size_t pos;
	size_t size;
} CTL_list_iterator;

static inline int CTL_list_new(CTL_list *handle);
static inline void CTL_list_delete(CTL_list *handle);
static inline void CTL_vector_clear(CTL_list *handle);

static inline int CTL_list_push_front(CTL_list *handle, type data);
static inline int CTL_list_pop_front(CTL_list *handle);

static inline int CTL_list_push_back(CTL_list *handle, type data);
static inline int CTL_list_pop_back(CTL_list *handle);

static inline int CTL_list_insert(CTL_list *handle, CTL_list_iterator iterator, type data);
void CTL_list_erase(CTL_list *handle, CTL_list_iterator iterator);

int CTL_list_at(CTL_list *handle, CTL_list_iterator *iterator, size_t pos);
static inline int CTL_list_iterator_add(CTL_list_iterator *handle, size_t pos);
static inline int CTL_list_iterator_sub(CTL_list_iterator *handle, size_t pos);

/*实现*/
static inline int CTL_list_new(CTL_list *handle)
{
	handle->size = 0;

	handle->list.head.next = NULL;
	handle->list.head.prior = NULL;

	handle->list.tail = &handle->list.head;

	return 0;
}

static inline void CTL_list_delete(CTL_list *handle)
{
	CTL_vector_clear(handle);
}

static inline void CTL_vector_clear(CTL_list *handle)
{
	for (__CTL_DuLNode *node = handle->list.head.next; node;)
	{
		__CTL_DuLNode *next_node = node->next;
		free(node);
		node = next_node;
	}
	handle->size = 0;

	handle->list.head.next = NULL;
	handle->list.head.prior = NULL;

	handle->list.tail = &handle->list.head;
}

static inline int CTL_list_push_front(CTL_list *handle, type data)
{
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)malloc(sizeof(__CTL_DuLNode));
	if (!new_node)
		return CTL_MALLOC_FAILED;

	new_node->data = data;
	new_node->next = handle->list.head.next;

	if (handle->list.head.next)
		handle->list.head.next->prior = new_node;

	new_node->prior = &handle->list.head;
	handle->list.head.next = new_node;

	if (handle->list.tail == &handle->list.head)
		handle->list.tail = new_node;

	++handle->size;
	return 0;
}

static inline int CTL_list_pop_front(CTL_list *handle)
{
	if (handle->size <= 0)
		return -1;

	--handle->size;
	__CTL_DuLNode *old_node = handle->list.head.next;
	handle->list.head.next = old_node->next;

	if (old_node->next)
		old_node->next->prior = &handle->list.head;
	else
		handle->list.tail = &handle->list.head;

	free(old_node);
}

static inline int CTL_list_push_back(CTL_list *handle, type data)
{
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)malloc(sizeof(__CTL_DuLNode));
	if (!new_node)
		return CTL_MALLOC_FAILED;

	new_node->data = data;
	new_node->next = NULL;

	handle->list.tail->next = new_node;
	new_node->prior = handle->list.tail;
	handle->list.tail = new_node;

	++handle->size;
	return 0;
}

static inline int CTL_list_pop_back(CTL_list *handle)
{
	if (handle->size <= 0)
		return -1;

	--handle->size;
	handle->list.tail->prior->next = NULL;
	__CTL_DuLNode *old_node = handle->list.tail;
	handle->list.tail = handle->list.tail->prior;

	free(old_node);
	return 0;
}

static inline int CTL_list_insert(CTL_list *handle, CTL_list_iterator iterator, type data)
{
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)malloc(sizeof(__CTL_DuLNode));
	if (!new_node)
		return CTL_MALLOC_FAILED;

	new_node->prior = iterator.node->prior;
	new_node->next = iterator.node;
	iterator.node->prior = new_node;
	++handle->size;
	return 0;
}

void CTL_list_erase(CTL_list *handle, CTL_list_iterator iterator)
{
	--handle->size;

	iterator.node->prior->next = iterator.node->next;
	if (iterator.node->next != NULL)
		iterator.node->next->prior = iterator.node->prior;

	free(iterator.node);
}

int CTL_list_at(CTL_list *handle, CTL_list_iterator *iterator, size_t pos)
{
	if (pos > handle->size - 1 || pos < 0)
		return CTL_OUT_OF_RANGE;

	__CTL_DuLNode *node;
	if (pos + 1 < handle->size / 2)
	{
		node = &handle->list.head;

		for (size_t i = 0; node && i <= pos; ++i, node = node->next)
			;
	}
	else
	{
		node = handle->list.tail;
		for (size_t i = handle->size; node && i > pos + 1; --i, node = node->prior)
			;
	}

	iterator->size = handle->size;
	iterator->pos = pos;
	iterator->node = node;
	iterator->data = &iterator->node->data;
	return 0;
}

static inline int CTL_list_iterator_add(CTL_list_iterator *handle, size_t pos)
{
	if (handle->pos + pos > handle->size - 1)
		return CTL_OUT_OF_RANGE;

	__CTL_DuLNode *node = handle->node;
	for (size_t i = 0; node && i < pos; ++i, node = node->next)
		;

	handle->pos += pos;
	handle->node = node;
	handle->data = &node->data;
	return 0;
}

static inline int CTL_list_iterator_sub(CTL_list_iterator *handle, size_t pos)
{
	if (handle->pos - pos < 0)
		return CTL_OUT_OF_RANGE;

	__CTL_DuLNode *node = handle->node;
	for (size_t i = 0; node && i < pos; ++i, node = node->prior)
		;

	handle->pos -= pos;
	handle->node = node;
	handle->data = &node->data;
	return 0;
}
