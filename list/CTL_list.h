#pragma once

#include <malloc.h>
#include <stdint.h>

#include "../error/CTL_error.h"

#define CTL_LIST(type)                                                                                               \
	/*声明*/                                                                                                       \
	typedef struct __CTL_DuLNode_##type                                                                              \
	{                                                                                                                \
		type data;                                                                                                   \
		struct __CTL_DuLNode_##type *prior;                                                                          \
		struct __CTL_DuLNode_##type *next;                                                                           \
	} __CTL_DuLNode_##type;                                                                                          \
                                                                                                                     \
	typedef struct                                                                                                   \
	{                                                                                                                \
		struct                                                                                                       \
		{                                                                                                            \
			__CTL_DuLNode_##type head;                                                                               \
			__CTL_DuLNode_##type *tail;                                                                              \
		} list;                                                                                                      \
                                                                                                                     \
		uint64_t size;                                                                                               \
	} CTL_list_##type;                                                                                               \
                                                                                                                     \
	typedef struct                                                                                                   \
	{                                                                                                                \
		__CTL_DuLNode_##type *node;                                                                                  \
		type *data;                                                                                                  \
		uint64_t pos;                                                                                                \
		uint64_t size;                                                                                               \
	} CTL_list_iterator_##type;                                                                                      \
                                                                                                                     \
	static inline int CTL_list_new_##type(CTL_list_##type *handle);                                                  \
	static inline int CTL_list_push_front_##type(CTL_list_##type *handle, type data);                                \
	static inline int CTL_list_pop_front_##type(CTL_list_##type *handle);                                            \
	static inline int CTL_list_push_back_##type(CTL_list_##type *handle, type data);                                 \
	static inline int CTL_list_pop_back_##type(CTL_list_##type *handle);                                             \
	static inline int CTL_list_insert_##type(CTL_list_##type *handle, CTL_list_iterator_##type iterator, type data); \
	void CTL_list_erase_##type(CTL_list_##type *handle, CTL_list_iterator_##type iterator);                          \
	static inline void CTL_list_delete_##type(CTL_list_##type *handle);                                              \
                                                                                                                     \
	int CTL_list_at_##type(CTL_list_##type *handle, CTL_list_iterator_##type *iterator, uint64_t pos);               \
	static inline int CTL_list_iterator_add_##type(CTL_list_iterator_##type *handle, uint64_t pos);                  \
	static inline int CTL_list_iterator_sub_##type(CTL_list_iterator_##type *handle, uint64_t pos);                  \
                                                                                                                     \
	/*实现*/                                                                                                       \
	static inline int CTL_list_new_##type(CTL_list_##type *handle)                                                   \
	{                                                                                                                \
		handle->size = 0;                                                                                            \
                                                                                                                     \
		handle->list.head.next = NULL;                                                                               \
		handle->list.head.prior = NULL;                                                                              \
                                                                                                                     \
		handle->list.tail = &handle->list.head;                                                                      \
                                                                                                                     \
		return 0;                                                                                                    \
	}                                                                                                                \
                                                                                                                     \
	static inline int CTL_list_push_front_##type(CTL_list_##type *handle, type data)                                 \
	{                                                                                                                \
		__CTL_DuLNode_##type *new_node = (__CTL_DuLNode_##type *)malloc(sizeof(__CTL_DuLNode_##type));               \
		if (!new_node)                                                                                               \
			return CTL_MALLOC_FAILED;                                                                                \
                                                                                                                     \
		++handle->size;                                                                                              \
		new_node->data = data;                                                                                       \
		new_node->next = handle->list.head.next;                                                                     \
                                                                                                                     \
		if (handle->list.head.next)                                                                                  \
			handle->list.head.next->prior = new_node;                                                                \
                                                                                                                     \
		new_node->prior = &handle->list.head;                                                                        \
		handle->list.head.next = new_node;                                                                           \
                                                                                                                     \
		if (handle->list.tail == &handle->list.head)                                                                 \
			handle->list.tail = new_node;                                                                            \
                                                                                                                     \
		return 0;                                                                                                    \
	}                                                                                                                \
                                                                                                                     \
	static inline int CTL_list_pop_front_##type(CTL_list_##type *handle)                                             \
	{                                                                                                                \
		if (handle->size <= 0)                                                                                       \
			return -1;                                                                                               \
                                                                                                                     \
		--handle->size;                                                                                              \
		__CTL_DuLNode_##type *old_node = handle->list.head.next;                                                     \
		handle->list.head.next = old_node->next;                                                                     \
                                                                                                                     \
		if (old_node->next)                                                                                          \
			old_node->next->prior = &handle->list.head;                                                              \
		else                                                                                                         \
			handle->list.tail = &handle->list.head;                                                                  \
                                                                                                                     \
		free(old_node);                                                                                              \
	}                                                                                                                \
                                                                                                                     \
	static inline int CTL_list_push_back_##type(CTL_list_##type *handle, type data)                                  \
	{                                                                                                                \
		__CTL_DuLNode_##type *new_node = (__CTL_DuLNode_##type *)malloc(sizeof(__CTL_DuLNode_##type));               \
		if (!new_node)                                                                                               \
			return CTL_MALLOC_FAILED;                                                                                \
                                                                                                                     \
		++handle->size;                                                                                              \
		new_node->data = data;                                                                                       \
		new_node->next = NULL;                                                                                       \
                                                                                                                     \
		handle->list.tail->next = new_node;                                                                          \
		new_node->prior = handle->list.tail;                                                                         \
		handle->list.tail = new_node;                                                                                \
                                                                                                                     \
		return 0;                                                                                                    \
	}                                                                                                                \
                                                                                                                     \
	static inline int CTL_list_pop_back_##type(CTL_list_##type *handle)                                              \
	{                                                                                                                \
		if (handle->size <= 0)                                                                                       \
			return -1;                                                                                               \
                                                                                                                     \
		--handle->size;                                                                                              \
		handle->list.tail->prior->next = NULL;                                                                       \
		__CTL_DuLNode_##type *old_node = handle->list.tail;                                                          \
		handle->list.tail = handle->list.tail->prior;                                                                \
                                                                                                                     \
		free(old_node);                                                                                              \
	}                                                                                                                \
                                                                                                                     \
	static inline int CTL_list_insert_##type(CTL_list_##type *handle, CTL_list_iterator_##type iterator, type data)  \
	{                                                                                                                \
		++handle->size;                                                                                              \
		__CTL_DuLNode_##type *new_node = (__CTL_DuLNode_##type *)malloc(sizeof(__CTL_DuLNode_##type));               \
		if (!new_node)                                                                                               \
			return CTL_MALLOC_FAILED;                                                                                \
                                                                                                                     \
		new_node->prior = iterator.node->prior;                                                                      \
		new_node->next = iterator.node;                                                                              \
		iterator.node->prior = new_node;                                                                             \
		return 0;                                                                                                    \
	}                                                                                                                \
                                                                                                                     \
	void CTL_list_erase_##type(CTL_list_##type *handle, CTL_list_iterator_##type iterator)                           \
	{                                                                                                                \
		--handle->size;                                                                                              \
                                                                                                                     \
		iterator.node->prior->next = iterator.node->next;                                                            \
		if (iterator.node->next != NULL)                                                                             \
			iterator.node->next->prior = iterator.node->prior;                                                       \
                                                                                                                     \
		free(iterator.node);                                                                                         \
		return;                                                                                                      \
	}                                                                                                                \
                                                                                                                     \
	static inline void CTL_list_delete_##type(CTL_list_##type *handle)                                               \
	{                                                                                                                \
		for (__CTL_DuLNode_##type *node = handle->list.head.prior; node;)                                            \
		{                                                                                                            \
			__CTL_DuLNode_##type *next_node = node->next;                                                            \
			free(node);                                                                                              \
			node = next_node;                                                                                        \
		}                                                                                                            \
                                                                                                                     \
		return;                                                                                                      \
	}                                                                                                                \
                                                                                                                     \
	int CTL_list_at_##type(CTL_list_##type *handle, CTL_list_iterator_##type *iterator, uint64_t pos)                \
	{                                                                                                                \
		if (pos > handle->size - 1 || pos < 0)                                                                       \
			return CTL_OUT_OF_RANGE;                                                                                 \
                                                                                                                     \
		__CTL_DuLNode_##type *node = &handle->list.head;                                                             \
                                                                                                                     \
		for (uint64_t i = 0; node && i <= pos; ++i, node = node->next)                                               \
			;                                                                                                        \
                                                                                                                     \
		iterator->size = handle->size;                                                                               \
		iterator->pos = pos;                                                                                         \
		iterator->node = node;                                                                                       \
		iterator->data = &iterator->node->data;                                                                      \
		return 0;                                                                                                    \
	}                                                                                                                \
                                                                                                                     \
	static inline int CTL_list_iterator_add_##type(CTL_list_iterator_##type *handle, uint64_t pos)                   \
	{                                                                                                                \
		if (handle->pos + pos > handle->size - 1)                                                                    \
			return CTL_OUT_OF_RANGE;                                                                                 \
                                                                                                                     \
		__CTL_DuLNode_##type *node = handle->node;                                                                   \
		for (uint64_t i = 0; node && i < pos; ++i, node = node->next)                                                \
			;                                                                                                        \
                                                                                                                     \
		handle->pos += pos;                                                                                          \
		handle->node = node;                                                                                         \
		handle->data = &node->data;                                                                                  \
		return 0;                                                                                                    \
	}                                                                                                                \
                                                                                                                     \
	static inline int CTL_list_iterator_sub_##type(CTL_list_iterator_##type *handle, uint64_t pos)                   \
	{                                                                                                                \
		if (handle->pos - pos < 0)                                                                                   \
			return CTL_OUT_OF_RANGE;                                                                                 \
                                                                                                                     \
		__CTL_DuLNode_##type *node = handle->node;                                                                   \
		for (uint64_t i = 0; node && i < pos; ++i, node = node->prior)                                               \
			;                                                                                                        \
                                                                                                                     \
		handle->pos -= pos;                                                                                          \
		handle->node = node;                                                                                         \
		handle->data = &node->data;                                                                                  \
		return 0;                                                                                                    \
	}
