#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "../public/CTL_public.h"
#include "../allocator/CTL_allocator.h"

typedef int type;

typedef struct
{
    type *data;
} CTL_vector_iterator;

typedef struct
{
    CTL_vector_iterator begin;
    CTL_vector_iterator end;
    size_t size;
    size_t capacity;
} CTL_vector;

void CTL_vector_new(CTL_vector *handle, size_t size);

void CTL_vector_delete(CTL_vector *handle);

void CTL_vector_clear(CTL_vector *handle);

void CTL_vector_push_back(CTL_vector *handle, type data);

void CTL_vector_pop_back(CTL_vector *handle);

void CTL_vector_insert(CTL_vector *handle, const CTL_vector_iterator *iterator, type data);

void CTL_vector_erase(CTL_vector *handle, const CTL_vector_iterator *iterator);

CTL_vector_iterator CTL_vector_at(const CTL_vector *handle, size_t pos);

CTL_vector_iterator CTL_vector_iterator_move(const CTL_vector_iterator *handle, size_t pos, bool front);

bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

bool CTL_vector_iterator_more(const CTL_vector_iterator *left, const CTL_vector_iterator *right);
