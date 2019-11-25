#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "./CTL_array.h"

void CTL_array_new(CTL_array *handle, type *begin, type *end)
{
    handle->begin.data = begin;
    handle->end.data = end;
}

void CTL_array_clear(CTL_array *handle)
{
    handle->end.data = handle->begin.data;
}

void CTL_array_push_back(CTL_array *handle, type data)
{
    *handle->end.data++ = data;
}

void CTL_array_pop_back(CTL_array *handle)
{
    --handle->end.data;
}

void CTL_array_insert(CTL_array *handle, const CTL_array_iterator *iterator, type data)
{
    memmove(iterator->data + 1, iterator->data, sizeof(type) * (handle->end.data - iterator->data));
    *iterator->data = data;
    ++handle->end.data;
}

void CTL_array_erase(CTL_array *handle, const CTL_array_iterator *iterator)
{
    memmove(iterator->data, iterator->data + 1, sizeof(type) * (handle->end.data - iterator->data - 1));
    --handle->end.data;
}

CTL_array_iterator CTL_array_at(const CTL_array *handle, size_t pos)
{
    CTL_array_iterator result;
    result.data = handle->begin.data + pos;
    return result;
}

CTL_array_iterator CTL_array_iterator_move(const CTL_array_iterator *handle, size_t pos, bool sub)
{
    CTL_array_iterator result;

    if (sub)
    {
        result.data = handle->data - pos;
    }
    else
    {
        result.data = handle->data + pos;
    }

    return result;
}

bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return left->data == right->data;
}

ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return left->data - right->data;
}

bool CTL_array_iterator_more(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return left->data > right->data;
}