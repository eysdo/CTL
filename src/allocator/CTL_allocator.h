#pragma once

#include <stdbool.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

int debug_mem = 0;

void (*CTL_malloc_handler)() = NULL; //内存分配失败 处理函数

static inline void *allocate(size_t size); //申请内存

static inline void *reallocate(void *old_ptr, size_t old_size, size_t new_size); //重新分配内存

static inline void deallocate(void *ptr, size_t size); //释放内存

#include "./CTL_allocator.c"