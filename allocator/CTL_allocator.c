#include "CTL_allocator.h"

//一级分配器
static inline void *CTL_malloc(size_t size);
static inline void *CTL_remalloc(void *old_ptr, size_t size);
static inline void CTL_free(void *ptr);

static inline void *CTL_malloc(size_t size)
{
    void *result = malloc(size);

    while (!result) //oom处理
    {
        assert(CTL_malloc_handler); //没有自定义处理 直接断言
        CTL_malloc_handler();
        result = malloc(size);
    }
    return result;
}

static inline void *CTL_remalloc(void *old_ptr, size_t size)
{
    void *result = realloc(old_ptr, size);

    while (!result) //oom处理
    {
        assert(CTL_malloc_handler); //没有自定义处理 直接断言
        CTL_malloc_handler();
        result = realloc(old_ptr, size);
    }
    return result;
}

static inline void CTL_free(void *ptr)
{
    free(ptr);
}

//二级分配器
#define ALIGN 8                      //区块上调边界
#define MAX_BYTES 128                //区块最大上限
#define NFREELISTS MAX_BYTES / ALIGN //free list 数量

#define ROUND_UP(bytes) (((bytes) + ALIGN - 1) & ~(ALIGN - 1))      //将bytes 上调至ALIGN 的倍数
#define FREELIST_INDEX(bytes) ((((bytes) + ALIGN - 1) / ALIGN) - 1) //根据bytes大小 决定free list

typedef union obj //free list 节点
{
    union obj *free_list_link; //指向下一个节点
    char client_data[1];       //指向内存块
} obj;

static inline void *refill(size_t size); //free list 填充函数

static inline void *chunk_alloc(size_t size, int *nobjs); //该函数 负责向内存池 索要 内存

static obj *free_list[NFREELISTS] = {NULL};

static char *begin_free = 0;
static char *end_free = 0;
static size_t heap_size = 0;

static inline void *allocate(size_t size)
{
    obj *result;

    if (size > MAX_BYTES)
    {
        //申请内存 大于MAX_BYTES 直接交给 一级分配器
        return CTL_malloc(size);
    }
    //找到合适的 free list
    obj **my_free_list = free_list + FREELIST_INDEX(size);
    result = *my_free_list;

    //如果 没用可以内存块 就重新填重 free list
    if (!result)
    {
        result = refill(ROUND_UP(size));
        return result;
    }
    *my_free_list = result->free_list_link;
    return result;
}

static inline void *reallocate(void *old_ptr, size_t old_size, size_t new_size)
{
    //申请和释放 都大于 MAX_BYTES 交给 一级分配器处理
    if (old_size > MAX_BYTES && new_size > MAX_BYTES)
    {
        return CTL_remalloc(old_ptr, new_size);
    }
    else
    {
        void *new_ptr = allocate(new_size);
        memcpy(new_ptr, old_ptr, new_size > old_size ? new_size : old_size);
        deallocate(old_ptr, old_size);
        return new_ptr;
    }
}

static inline void deallocate(void *ptr, size_t size)
{
    //释放内存 大于 MAX_BYTES 直接 交给 一级分配器
    if (size > MAX_BYTES)
    {
        CTL_free(ptr);
    }
    else
    {
        //将其交给 合适的 free list
        obj **my_free_list = free_list + FREELIST_INDEX(size);
        ((obj *)ptr)->free_list_link = *my_free_list;
        *my_free_list = (obj *)ptr;
    }
}

static inline void *refill(size_t size)
{
    obj *result;
    //申请 20个 size 大小区块 具体过程由chunk_alloc 函数执行
    int nobjs = 20;
    char *chunk = chunk_alloc(size, &nobjs);

    //如果只获得了一个区块 就将这个区块直接返回给用户 此时free_list中无新节点
    if (nobjs == 1)
    {
        return (chunk);
    }
    //否则就准备 把新区块 加入 free list
    obj **my_free_list = free_list + FREELIST_INDEX(size);
    //第一个区块块 返回给用户
    result = (obj *)chunk;
    obj *next_obj = *my_free_list = (obj *)(chunk + size);

    //挂载区块操作
    for (size_t i = 0;; ++i)
    {
        //循环从1开始，因为第一区块已经返回给用户了
        obj *current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + size);

        if (nobjs - 1 == i)
        {
            current_obj->free_list_link = NULL;
            break;
        }
        else
        {
            current_obj->free_list_link = next_obj;
        }
    }
    return result;
}

static inline void *chunk_alloc(size_t size, int *nobjs)
{
    void *result;
    //要分配的 内存 数量
    size_t bytes_total = size * (*nobjs);
    //内存池 剩余空间
    size_t bytes_left = end_free - begin_free;

    //内存池 剩余空间 足够需求
    if (bytes_left >= bytes_total)
    {
        result = begin_free;
        begin_free += bytes_total;
        return result;
    }
    else if (bytes_left > size)
    {
        //内存池 空间不足 但至少够一个区块的
        *nobjs = (int)(bytes_left / size);
        bytes_total = size * (*nobjs);
        result = begin_free;
        begin_free += bytes_total;
        return result;
    }
    else
    {
        //如果 一个区块 空间都不足够了
        size_t bytes_to_get = 2 * bytes_total + ROUND_UP(heap_size >> 4);

        //如果 内存池还有剩余 就把它们 放入 free list 中
        if (bytes_total > 0)
        {
            obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *)begin_free)->free_list_link = *my_free_list;
            *my_free_list = (obj *)begin_free;
        }

        begin_free = (char *)malloc(bytes_to_get); //调用malloc 申请新的内存池
        if (!begin_free)
        {
            //如果 内存空间不足
            //去free list看看 还有没有 较大的区块 将它先返回 给用户 使用
            for (size_t i = size; i < MAX_BYTES; i += ALIGN)
            {
                obj **my_free_list = free_list + FREELIST_INDEX(i);
                obj *p = *my_free_list;
                if (p)
                {
                    //free list 中 还有 较大的区块 将它调整出来
                    *my_free_list = p->free_list_link;
                    begin_free = (char *)p;
                    end_free = begin_free + i;
                    //递归 调整free list
                    return chunk_alloc(size, nobjs);
                }
            }
            //如果 彻底 没有空间了 这时候 交给一级分配器 处理
            //看看 OOM机制 是否有效
            //end_free = 0;
            begin_free = CTL_malloc(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = begin_free + bytes_to_get;
        //递归 调整free list
        return chunk_alloc(size, nobjs);
    }
}