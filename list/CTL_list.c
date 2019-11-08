#include <malloc.h>
#include <stdint.h>

#include "CTL_list_iterator.h"
#include "CTL_list.h"

int CTL_list_new(CTL_list *handle)
{
	handle->size = 0;

	handle->list.head.next = NULL;
	handle->list.head.prior = NULL;

	handle->list.tail = &handle->list.head;

	return 0;
}

int CTL_list_push(CTL_list *handle, void *data)
{
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)malloc(sizeof(__CTL_DuLNode));
	if (!new_node)
		return -1;

	++handle->size;
	new_node->data = data;
	new_node->next = NULL;

	handle->list.tail->next = new_node;
	new_node->prior = handle->list.tail;
	handle->list.tail = new_node;

	return 0;
}

int CTL_list_pop(CTL_list *handle)
{
	if (handle->size <= 0)
		return -1;

	--handle->size;
	handle->list.tail->prior->next = NULL;
	__CTL_DuLNode *old_node = handle->list.tail;
	handle->list.tail = handle->list.tail->prior;

	free(old_node);
}

int CTL_list_insert(CTL_list *handle, CTL_list_iterator iterator, void *data)
{
	++handle->size;
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)malloc(sizeof(__CTL_DuLNode));
	if (!new_node)
		return -1;

	new_node->prior = iterator.node->prior;
	new_node->next = iterator.node;
	iterator.node->prior = new_node;
	return 0;
}

int CTL_list_erase(CTL_list *handle, CTL_list_iterator iterator)
{
	if (handle->size <= 0)
		return -1;
	--handle->size;

	iterator.node->prior->next = iterator.node->next;
	if (iterator.node->next != NULL)
		iterator.node->next->prior = iterator.node->prior;

	free(iterator.node);
}

int CTL_list_delete(CTL_list *handle)
{
	for (__CTL_DuLNode *node = handle->list.head.prior; node; )
	{
		__CTL_DuLNode *old_node = node->next;
		free(node);
		node = old_node;
	}

	return 0;
}

int CTL_list_take_iterator(CTL_list *handle, CTL_list_iterator *iterator, uint64_t pos)
{
	if (pos > handle->size || pos == 0)
		return -1;

	__CTL_DuLNode *node = &handle->list.head;

	for (uint64_t i = 0; node && i < pos; ++i, node = node->next)
		;

	CTL_list_iterator ret;
	iterator->pos = pos;
	iterator->node = node;
	return 0;
}