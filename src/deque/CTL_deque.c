#include "../allocator/CTL_allocator.h"
#include "../allocator/CTL_allocator.c"

#include "../public/CTL_public.h"

#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef int type;

typedef struct
{
    type *data;  //当前所指的位置
    type *cur;   //此迭代器 所指缓存区 的现行元素
    type *first; //此迭代器 所指缓存区 的头地址
    type *last;  //此迭代器 所指缓存区 的尾地址(含备用空间)
    type **node; //指向map 管控中心
} CTL_deque_iterator;

typedef struct
{
    type **map;
    size_t map_size;
    size_t buf_size;
    //size_t size;
    CTL_deque_iterator begin; //第一个指针
    CTL_deque_iterator end;   //最后一个指针
} CTL_deque;

#define set_node(dest, src, buf_size)    \
    (dest).node = (src);                 \
    (dest).first = *(src);               \
    (dest).last = *(src) + (buf_size)-1;

void CTL_deque_new(CTL_deque *handle, size_t buf_size)
{
    handle->buf_size = buf_size;
    //一个map至少 拥有8个节点
    handle->map_size = 8;
    handle->map = (type **)CTL_allocate(sizeof(type *) * handle->map_size);

    //先分配一个缓存区
    type **start = handle->map + ((handle->map_size - 1) / 2);
    type **finish = start;
    *start = CTL_allocate(sizeof(type) * handle->buf_size);
    //设置 begin和end 两个迭代器
    set_node(handle->begin, start, handle->buf_size);
    handle->begin.cur = *start;
    set_node(handle->end, finish, handle->buf_size);
    handle->end.cur = *finish;
}

void reallocate_map(CTL_deque *handle, size_t nodes_to_add, bool front)
{
    size_t num_old_nodes = handle->end.node - handle->begin.node + 1;
    size_t num_new_nodes = num_old_nodes + nodes_to_add;

    type **new_start;

    if (handle->map_size > 2 * num_new_nodes)
    {
        //map空间足够
        new_start = handle->map + (handle->map_size - num_new_nodes) / 2 + (front ? nodes_to_add : 0);

        memmove(new_start, handle->begin.node, sizeof(type *) * (handle->end.node - handle->begin.node + 1));
    }
    else
    {
        //重新 分配 map空间
        size_t new_map_size = handle->map_size + (handle->map_size > nodes_to_add ? handle->map_size : nodes_to_add);
        type **new_map = CTL_allocate(sizeof(type *) * new_map_size);

        new_start = new_map + (handle->map_size - num_new_nodes) / 2 + (front ? nodes_to_add : 0);

        memmove(new_start, handle->begin.node, sizeof(type *) * (handle->end.node - handle->begin.node + 1));
        CTL_deallocate(handle->map, sizeof(type *) * handle->map_size);

        handle->map = new_map;
        handle->map_size = new_map_size;
    }

    set_node(handle->begin, new_start, handle->buf_size);
    handle->begin.cur = *new_start;
    set_node(handle->end, new_start + num_old_nodes - 1, handle->buf_size);
    handle->end.cur = *(new_start + num_old_nodes - 1);
}

void push_aux(CTL_deque *handle, bool front)
{
    if (front)
    {
        if (1 > handle->begin.node - handle->map)
        {
            //前端 空间不足
            reallocate_map(handle, 1, true);
        }
        //分配一个新节点
        *(handle->begin.node - 1) = (type *)CTL_allocate(sizeof(type) * handle->buf_size);
        //设置begin 迭代器
        --handle->begin.node;
        set_node(handle->begin, handle->begin.node, handle->buf_size);
        handle->begin.cur = handle->begin.last - 1;
    }
    else
    {
        if (2 > handle->map_size - (handle->end.node - handle->map))
        {
            //尾端 空间不足
            reallocate_map(handle, 1, false);
        }
        //分配一个新节点
        *(handle->end.node + 1) = (type *)CTL_allocate(sizeof(type) * handle->buf_size);
        //设置end 迭代器
        ++handle->end.node;
        set_node(handle->end, handle->end.node, handle->buf_size);
        handle->end.cur = handle->end.first;
    }
}

void CTL_deque_push(CTL_deque *handle, int data, bool front)
{
    if (front)
    {
        if (handle->begin.cur != handle->begin.first)
        {
            //第一个缓存区 尚有 空间
            *(--handle->begin.cur) = data;
        }
        else
        {
            push_aux(handle, true);
            *handle->begin.cur = data;
        }
    }
    else
    {
        if (handle->end.cur != handle->end.last - 1)
        {
            //最后一个缓存区 尚有 空间
            *(handle->end.cur++) = data;
        }
        else
        {
            *handle->end.cur = data;
             push_aux(handle, false);
        }
    }
}

void pop_aux(CTL_deque *handle, bool front)
{
    if (front)
    {
        CTL_deallocate(handle->begin.node, sizeof(type) * handle->map_size);
        ++handle->begin.node;
        set_node(handle->begin, handle->begin.node, handle->buf_size);
        handle->begin.cur = handle->begin.first;
    }
    else
    {
        CTL_deallocate(handle->end.node, sizeof(type) * handle->map_size);
        --handle->end.node;
        set_node(handle->end, handle->end.node, handle->buf_size);
        handle->end.cur = handle->end.last - 1;
    }
}

void CTL_deque_pop(CTL_deque *handle, int *data, bool front)
{
    //对空deque操作的话 直接断言
    assert(handle->begin.cur != handle->end.cur);

    if (front)
    {
        *data = *handle->begin.cur;
        if (handle->begin.cur != handle->begin.last - 1)
        {
            //缓存区 更多元素
            //最后一个缓存区 尚有 一个空间
            ++handle->begin.cur;
        }
        else
        {
            //仅有一个元素
            pop_aux(handle, true);
        }
    }
    else
    {
        if (handle->end.cur != handle->end.first)
        {
            //缓存区 更多元素
            //最后一个缓存区 尚有 一个空间
            --handle->end.cur;
        }
        else
        {
            //仅有一个元素
            pop_aux(handle, false);
        }
        *data = *handle->end.cur;
    }
}

void CTL_deque_clear(CTL_deque *handle)
{
    //删除 第二个 - 倒数第二个 缓存区

    // 删除前后两个数组的元素.
    if (handle->begin.node != handle->end.node)
    {
        //释放缓存区
        for (type **node = handle->begin.node + 1; node <= handle->end.node; ++node)
        {
            CTL_deallocate(*node, sizeof(type) * handle->buf_size);
        }
    }

    //重新调整map
    type **start = handle->begin.node;
    type **finish = start;
    //设置 begin和end 两个迭代器
    set_node(handle->begin, start, handle->buf_size);
    handle->begin.cur = *start;
    set_node(handle->end, finish, handle->buf_size);
    handle->end.cur = *finish;
}

void CTL_deque_erase(CTL_deque *handle, CTL_deque_iterator iterator)
{

}

int main(void)
{

    CTL_deque handle;
    CTL_deque_new(&handle, 10);
    
    for (size_t i = 0; i < 100; i++)
    {
        CTL_deque_push(&handle, i, false);
    }

    //printf("%d %d\n", *handle.begin.cur, *handle.end.cur);
    
    
    int t = 0;
    for (size_t i = 0; i < 100; i++)
    {
        CTL_deque_pop(&handle, &t, true);
        printf("%d\n", t);
    }
    
    printf("%d\n", debug_mem);
    return 0;
}