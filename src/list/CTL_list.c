#include "CTL_list.h"

#define SET_BEGIN_AND_END(handle)                       \
	if ((handle).begin.node != (handle).list->next)       \
	{                                                   \
		(handle).begin.node = (handle).list->next;        \
		(handle).begin.data = &(handle).list->next->data; \
	}                                                   \
	if ((handle).end.node != (handle).list)               \
	{                                                   \
		(handle).end.node = (handle).list;                \
		(handle).end.data = &(handle).list->data;         \
	}

void CTL_list_new(CTL_list *handle)
{
	//构造 一个 头结点
	__CTL_DuLNode *node = (__CTL_DuLNode *)CTL_allocate(sizeof(__CTL_DuLNode));
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;
	SET_BEGIN_AND_END(*handle);
	handle->size = 0;
}

void CTL_list_delete(CTL_list *handle)
{
	CTL_list_clear(handle);
	CTL_deallocate(handle->list, sizeof(__CTL_DuLNode));
}

void CTL_list_clear(CTL_list *handle)
{
	__CTL_DuLNode *node;
	for (node = handle->list->next; node != handle->list;)
	{
		__CTL_DuLNode *next_node = node->next;
		CTL_deallocate(node, sizeof(__CTL_DuLNode));
		node = next_node;
	}

	handle->size = 0;
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;
	SET_BEGIN_AND_END(*handle);
}

void CTL_list_push_back(CTL_list *handle, type data)
{
	CTL_list_insert(handle, &handle->end, data);
}

void CTL_list_push_front(CTL_list *handle, type data)
{
	CTL_list_insert(handle, &handle->begin, data);
}

void CTL_list_pop_back(CTL_list *handle)
{
	assert(handle->size);
	CTL_list_erase(handle, &handle->end);
}

void CTL_list_pop_front(CTL_list *handle)
{
	assert(handle->size);
	CTL_list_erase(handle, &handle->begin);
}

void CTL_list_insert(CTL_list *handle, const CTL_list_iterator *iterator, type data)
{
	__CTL_DuLNode *new_node = (__CTL_DuLNode *)CTL_allocate(sizeof(__CTL_DuLNode));

	new_node->data = data;
	new_node->next = iterator->node;
	new_node->prior = iterator->node->prior;

	new_node->prior->next = new_node;
	iterator->node->prior = new_node;
	SET_BEGIN_AND_END(*handle);
	++handle->size;
}

void CTL_list_erase(CTL_list *handle, const CTL_list_iterator *iterator)
{
	iterator->node->prior->next = iterator->node->next;
	iterator->node->next->prior = iterator->node->prior;

	CTL_deallocate(iterator->node, sizeof(__CTL_DuLNode));
	SET_BEGIN_AND_END(*handle);
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

void transfer(const CTL_list_iterator *position, const CTL_list_iterator *first, const CTL_list_iterator *last)
{
	if (CTL_list_iterator_equal(position, last))
	{
		return;
	}
	last->node->prior->next = position->node;
	first->node->prior->next = last->node;
	position->node->prior->next = first->node;
	__CTL_DuLNode *tmp = position->node->prior;
	position->node->prior = last->node->prior;
	last->node->prior = first->node->prior;
	first->node->prior = tmp;
}

void splice(CTL_list *handle, const CTL_list_iterator *position, CTL_list *x, const CTL_list_iterator *first, const CTL_list_iterator *last)
{
	transfer(position, first, last);
	SET_BEGIN_AND_END(*handle);
	SET_BEGIN_AND_END(*x);
}

void merge(CTL_list *handle, CTL_list *x, bool (*fun)(type handle, type x))
{
	CTL_list_iterator first_1 = handle->begin;
	CTL_list_iterator last_1 = handle->end;

	CTL_list_iterator first_2 = x->begin;
	CTL_list_iterator last_2 = x->end;

	while (!CTL_list_iterator_equal(&first_1, &last_1) && !CTL_list_iterator_equal(&first_2, &last_2))
	{
		if(fun(*first_1.data, *first_2.data))
		{
			CTL_list_iterator next = CTL_list_iterator_move(&first_2, 1, false);
			transfer(&first_1, &first_2, &next);
			first_2  = next;
		}
		else
		{
			first_1 = CTL_list_iterator_move(&first_1, 1, false);
		}
	}

	if (!CTL_list_iterator_equal(&first_2, &last_2))
	{
		transfer(&last_1, &first_2, &last_2);
	}

	SET_BEGIN_AND_END(*handle);
	SET_BEGIN_AND_END(*x);
}
