#include "CTL_list.h"

void CTL_list_new(CTL_list *handle)
{
	//构造 一个 头结点
	__CTL_DuLNode *node = (__CTL_DuLNode *)CTL_allocate(sizeof(__CTL_DuLNode));
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;

	handle->begin.node = handle->list->next;
	handle->begin.data = &handle->list->next->data;

	handle->end.node = handle->list;
	handle->end.data = &handle->list->data;
	handle->size = 0;
}

void CTL_list_delete(CTL_list *handle)
{
	CTL_vector_clear(handle);
	CTL_deallocate(handle->list, sizeof(__CTL_DuLNode));
}

void CTL_vector_clear(CTL_list *handle)
{
	__CTL_DuLNode *node;
	for (node = handle->list->next; node != handle->list;)
	{
		__CTL_DuLNode *next_node = node->next;
		CTL_deallocate(handle->list, sizeof(__CTL_DuLNode));
		node = next_node;
	}

	handle->size = 0;
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;
}

void CTL_list_push_back(CTL_list *handle, type data)
{
	CTL_list_insert(handle, &handle->end, data);
	handle->begin.node = handle->list->next;
	handle->begin.data = &handle->list->next->data;

	handle->end.node = handle->list;
	handle->end.data = &handle->list->data;
}

void CTL_list_push_front(CTL_list *handle, type data)
{
	CTL_list_insert(handle, &handle->begin, data);
	handle->begin.node = handle->list->next;
	handle->begin.data = &handle->list->next->data;

	handle->end.node = handle->list;
	handle->end.data = &handle->list->data;
}

void CTL_list_pop_back(CTL_list *handle)
{
	assert(handle->size);
	CTL_list_erase(handle, &handle->end);
	handle->begin.node = handle->list->next;
	handle->begin.data = &handle->list->next->data;

	handle->end.node = handle->list;
	handle->end.data = &handle->list->data;
}

void CTL_list_pop_front(CTL_list *handle)
{
	assert(handle->size);
	CTL_list_erase(handle, &handle->begin);
	handle->begin.node = handle->list->next;
	handle->begin.data = &handle->list->next->data;

	handle->end.node = handle->list;
	handle->end.data = &handle->list->data;
}

void CTL_list_insert(CTL_list *handle, const CTL_list_iterator *iterator, type data)
{
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)CTL_allocate(sizeof(__CTL_DuLNode));

	new_node->data = data;
	new_node->next = iterator->node;
	new_node->prior = iterator->node->prior;

	new_node->prior->next = new_node;
	iterator->node->prior = new_node;

	++handle->size;
}

void CTL_list_erase(CTL_list *handle, const CTL_list_iterator *iterator)
{
	iterator->node->prior->next = iterator->node->next;
	iterator->node->next->prior = iterator->node->prior;

	CTL_deallocate(iterator->node, sizeof(__CTL_DuLNode));
	--handle->size;
}

CTL_list_iterator CTL_list_at(const CTL_list *handle, size_t pos)
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

CTL_list_iterator CTL_list_iterator_move(const CTL_list_iterator *handle, size_t pos, bool sub)
{
	CTL_list_iterator result;
	if (sub)
	{
		__CTL_DuLNode *node = handle->node;
		for (size_t i = 0; node && i < pos; ++i, node = node->prior)
			;
		result.node = node;
		result.data = &node->data;
	}
	else
	{
		__CTL_DuLNode *node = handle->node;
		for (size_t i = 0; i < pos; ++i, node = node->next)
			;
		result.node = node;
		result.data = &node->data;
	}

	return result;
}

bool CTL_list_iterator_equal(const CTL_list_iterator *left, const CTL_list_iterator *right)
{
	return left->node == right->node;
}