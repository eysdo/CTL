# STL 分配器
## 前言
  由于C语言中 没有class如此复杂的数据 我们不需要实现new 和 delete操作<br>
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
    当内存分配失败 也就是内存空间 不足时 程序会陷入死循环
    并把控制权 交给用户所定义的  内存分配失败 处理例程
    尝试让用户自己解决 该问题
  
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
    一级分配器直接调用 free释放内存

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

static void *refill(size_t size); //free list 填充函数

static void *chunk_alloc(size_t size, int *nobjs); //该函数 负责向内存池 索要 内存

static obj *free_list[NFREELISTS] = {NULL};

static char *begin_free = 0;
static char *end_free = 0;
static size_t heap_size = 0;
```
    看到上述声明 你可能略显懵逼
    这里先讲解一下二级分配器的内存池
![二级分配器 内存池示意图](https://github.com/inie0722/CTL/blob/preview/img/%E4%BA%8C%E7%BA%A7%E5%88%86%E9%85%8D%E5%99%A8%20%E5%86%85%E5%AD%98%E6%B1%A0%E7%A4%BA%E6%84%8F%E5%9B%BE.png?raw=true "二级分配器 内存池示意图")	
    二级分配器 维护着 一个内存池和16个free list, 16个free list 分别存放8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,bytes的内存块
    内存申请过程 大概如下 此图为简易过程
  