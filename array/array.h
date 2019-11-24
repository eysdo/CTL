#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#include "../public/CTL_public.h"

typedef int type;

typedef struct
{
    type *begin;
    type *end;
} CTL_array;

typedef struct
{
    type *data;
} CTL_array_iterator;

void CTL_array_new(CTL_array *handle, type *begin, type *end);

void CTL_array_clear(CTL_array *handle);

void CTL_array_push_back(CTL_array *handle, type data);

void CTL_array_pop_back(CTL_array *handle);

void CTL_array_insert(CTL_array *handle, CTL_array_iterator iterator, type data);

void CTL_array_erase(CTL_array *handle, CTL_array_iterator iterator);

CTL_array_iterator CTL_array_at(const CTL_array *handle, size_t pos);

CTL_array_iterator CTL_array_iterator_move(const CTL_array_iterator *handle, size_t pos, bool front);

bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right);

ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right);

bool CTL_array_iterator_more(const CTL_array_iterator *left, const CTL_array_iterator *right);