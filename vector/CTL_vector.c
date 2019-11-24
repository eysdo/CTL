#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "./CTL_vector.h"

void CTL_vector_new(CTL_vector *handle, size_t size)
{
    handle->size = 0;
    handle->capacity = size;
    handle->begin = (type *)allocate(sizeof(type) * size);
    handle->end = handle->begin;
}

void CTL_vector_delete(CTL_vector *handle)
{
    CTL_vector_clear(handle);
    deallocate(handle->begin, sizeof(type) * handle->capacity);
}

void CTL_vector_clear(CTL_vector *handle)
{
    handle->size = 0;
    handle->end = handle->begin;
}

void CTL_vector_push_back(CTL_vector *handle, type data)
{
    //空间 不足 扩充一倍
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
        memmove(ptr, handle->begin, handle->capacity * sizeof(type));
        deallocate(handle->begin, handle->capacity * sizeof(type));
        handle->begin = ptr;
        handle->capacity *= 2;
    }

    *handle->end++ = data;
    ++handle->size;
}

void CTL_vector_pop_back(CTL_vector *handle)
{
    --handle->end;
    --handle->size;
}

void CTL_vector_insert(CTL_vector *handle, const CTL_vector_iterator *iterator, type data)
{
    //空间不足扩充操作
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)allocate(2 * handle->capacity * sizeof(type));
        //拷贝前面的数据
        memmove(ptr, handle->begin, sizeof(type) * (iterator->data - handle->begin));
        //拷贝后面的数据 并空出一个位置
        memmove(ptr + (iterator->data - handle->begin) + 1, iterator->data, sizeof(type) * (handle->end - iterator->data));
        deallocate(handle->begin, handle->capacity * sizeof(type));
        handle->capacity *= 2;
        //迭代器失效
        ptr[(iterator->data - handle->begin)] = data;
        handle->end = ptr + (handle->end - handle->begin + 1);
        handle->begin = ptr;
    }
    else
    {
        memmove(iterator->data + 1, iterator->data, sizeof(type) * (handle->end - iterator->data));
        *iterator->data = data;
        ++handle->end;
    }
    ++handle->size;
}

void CTL_vector_erase(CTL_vector *handle, const CTL_vector_iterator *iterator)
{
    memmove(iterator->data, iterator->data + 1, sizeof(type) * (handle->end - iterator->data - 1));
    --handle->end;
    --handle->size;
}

CTL_vector_iterator CTL_vector_at(const CTL_vector *handle, size_t pos)
{
    CTL_vector_iterator result;
    result.data = handle->begin + pos;
    return result;
}

CTL_vector_iterator CTL_vector_iterator_move(const CTL_vector_iterator *handle, size_t pos, bool front)
{
    CTL_vector_iterator result;

    if (front)
    {
        result.data = handle->data - pos;
    }
    else
    {
        result.data = handle->data + pos;
    }

    return result;
}

bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left == right;
}

ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left->data - right->data;
}

bool CTL_vector_iterator_more(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left > right;
}
