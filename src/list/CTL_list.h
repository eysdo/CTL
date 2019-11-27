#pragma once

#include <malloc.h>
#include <stdint.h>
#include <assert.h>

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
	type *data;
	__CTL_DuLNode *node;
} CTL_list_iterator;

typedef struct
{
	__CTL_DuLNode *list;
	CTL_list_iterator begin;
	CTL_list_iterator end;
	size_t size;
} CTL_list;

void CTL_list_new(CTL_list *handle);

void CTL_list_delete(CTL_list *handle);

void CTL_vector_clear(CTL_list *handle);

void CTL_list_push(CTL_list *handle, type data, int direction);

void CTL_list_pop(CTL_list *handle, int direction);

void CTL_list_insert(CTL_list *handle, const CTL_list_iterator *iterator, type data);

void CTL_list_erase(CTL_list *handle, const CTL_list_iterator *iterator);

CTL_list_iterator CTL_list_at(const CTL_list *handle, size_t pos);

CTL_list_iterator CTL_list_iterator_move(const CTL_list_iterator *handle, size_t pos, bool sub);