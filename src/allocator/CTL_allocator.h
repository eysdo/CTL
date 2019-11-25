#pragma once

#include <stdbool.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

int CTL_debug_mem = 0;

void (*CTL_malloc_handler)() = NULL; //内存分配失败 处理函数 由用户自定义

void *CTL_allocate(size_t size); //申请内存

void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size); //重新分配内存

void CTL_deallocate(void *ptr, size_t size); //释放内存
