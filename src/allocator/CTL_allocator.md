# STL 分配器
## 前言
[源码目录](/src/allocator/)<br>
由于C语言中 没有class如此复杂的数据 我们不需要实现new 和 delete操作<br>
__这里先不考虑 多线程__<br>
所以 本章 直接讲解STL一级分配器和二级分配器<br>
<br>
在STL中分配器被分为 两种级别<br>

## 一级分配器
一级分配器 负责内存申请和释放 仅对malloc和free 进行了简单的封装<br>

__主要对外接口__
```c
static void *CTL_malloc(size_t size);
static void *CTL_remalloc(void *old_ptr, size_t size);
static void CTL_free(void *ptr);
void (*CTL_malloc_handler)() = NULL; //内存分配失败 处理函数 由用户自定义
// 在SIG STL中应该还有 oom处理函数 在这里被我直接 合并了
```

  __CTL_malloc__
```c
static void *CTL_malloc(size_t size)
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
```
当内存分配失败 也就是内存空间 不足时 程序会陷入死循环<br>
并把控制权 交给用户所定义的  内存分配失败 处理例程<br>
尝试让用户自己解决 该问题<br>
  
  __CTL_remalloc__
```c
static void *CTL_remalloc(void *old_ptr, size_t size)
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
```
    这里和上面类似

  __CTL_free__
```c
static void CTL_free(void *ptr)
{
    free(ptr);
}
```
一级分配器直接调用 free释放内存<br>

## 二级分配器
二级分配器 在一级分配器的基础上 增加了一个内存池 小于128bytes内存申请 直接通过二级分配器分配<br>
大于128bytes的申请 交给一级分配器 通过malloc申请<br>

__主要对外接口__
```c
void *CTL_allocate(size_t size); //申请内存

void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size); //重新分配内存

void CTL_deallocate(void *ptr, size_t size); //释放内存
```

__实现所用的 宏 函数 类型 变量 声明 如下__
```c
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
//这里先看一下 obj联合体 书上说此联合体为 一物两用 同时指向下一个节点 和 内存块
//其实可以理解为 一个语法糖 这里client_data 指向内存块
//free_list_link 也指向内存块 也就是 下一个节点的地址 存放在内存块中

static void *refill(size_t size); //free list 填充函数

static void *chunk_alloc(size_t size, int *nobjs); //该函数 负责向内存池 索要 内存

static obj *free_list[NFREELISTS] = {NULL};

```
看到上述声明 你可能略显懵逼<br>
这里先讲解一下二级分配器的内存池<br>
![二级分配器 内存池示意图](/img/%E4%BA%8C%E7%BA%A7%E5%88%86%E9%85%8D%E5%99%A8%20%E5%86%85%E5%AD%98%E6%B1%A0%E7%A4%BA%E6%84%8F%E5%9B%BE.png?raw=true "二级分配器 内存池示意图")	
    二级分配器 维护着 一个内存池和16个free list<br>
    16个free list分别存放8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,bytes的内存块<br>

  内存申请过程 大概如下 此图为简易过程<br>
![二级分配器 内存分配 简易过程](/img/%E4%BA%8C%E7%BA%A7%E5%88%86%E9%85%8D%E5%99%A8%20%E5%86%85%E5%AD%98%E5%88%86%E9%85%8D%20%E7%AE%80%E6%98%93%E8%BF%87%E7%A8%8B.png?raw=true "二级分配器 内存分配 简易过程")

__CTL_allocate__
```c
void *CTL_allocate(size_t size)
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

    //如果 没用可以内存块 就重新填充 free list
    if (!result)
    {
        result = (obj*)refill(ROUND_UP(size));
        return result;
    }
    *my_free_list = result->free_list_link;
    return result;
}
```
这里没什么好讲的 如上面 那个流程图一样<br>
free list 有节点返回第一个节点, 无节点调用refill分配节点<br>

__refill__
```c
static void *refill(size_t size)
{
    obj *result;
    //申请 20个 size 大小区块 具体过程由chunk_alloc 函数执行
    int nobjs = 20;
    char *chunk = (char*)chunk_alloc(size, &nobjs);

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
```
    refill 主要负责挂载节点 具体申请过程 由chunk_alloc 执行

__CTL_deallocate__
```c
void CTL_deallocate(void *ptr, size_t size)
{
    ++CTL_debug_mem;
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
```
    大于128bytes 的内存块 直接交给 一级配置器 释放
    小于128bytes 的内存块 将其挂载到合适的 free list上 以便再次使用

__CTL_reallocate__
```c
void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size)
{
    //申请和释放 都大于 MAX_BYTES 交给 一级分配器处理
    if (old_size > MAX_BYTES && new_size > MAX_BYTES)
    {
        return CTL_remalloc(old_ptr, new_size);
    }
    else
    {
        void *new_ptr = CTL_allocate(new_size);
        memcpy(new_ptr, old_ptr, new_size > old_size ? new_size : old_size);
        CTL_deallocate(old_ptr, old_size);
        return new_ptr;
    }
}
```

内存池实现<br>
---
在看chunk_allo 源码前 先看一下 二级分配器工作原理 示例图<br>
![二级分配器工作原理](/img/%E4%BA%8C%E7%BA%A7%E5%88%86%E9%85%8D%E5%99%A8%20%E5%B7%A5%E4%BD%9C%E5%8E%9F%E7%90%86.png?raw=true "二级分配器工作原理")

__chunk_alloc__
```c
//这三个变量 负责管理内存池
static char *begin_free = 0;// 内存池的首地址
static char *end_free = 0;  //内存池的结束地址
static size_t heap_size = 0;//大小

static void *chunk_alloc(size_t size, int *nobjs)
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
        if (bytes_left > 0)
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
            begin_free = (char*)CTL_malloc(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = begin_free + bytes_to_get;
        //递归 调整free list
        return chunk_alloc(size, nobjs);
    }
}
```
读者 此时可能 有几个疑问<br>
---
    Q1 为什么 最小内存块 是8bytes
    A1 64位系统下 指针至少占用8bytes

    Q2 size_t bytes_to_get = 2 * bytes_total + ROUND_UP(heap_size >> 4);
        此行代码 为何要 >>4位
    A2 这里仅需要少量附加内存 因为内存池 直至程序结束 才被释放

更多 疑问 欢迎留言 共同讨论<br>

参考文献<br>
---
[STL源码剖析](https://item.jd.com/11821611.html)<br>
[GitHub: FunctionDou SIG STL源码分析](https://github.com/FunctionDou/STL)<br>
[CSDN: Amberda STL-----浅析二级空间配置器](https://blog.csdn.net/qq_34021920/article/details/80427568)<br>
