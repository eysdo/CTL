# array
## 前言
  本章 讲解的array 和 STL中的array 略有不同, 它更像是一个 不支持动态扩展的vector<br>
  array的实现 十分简单, 所以 这里也同时介绍一下 本库设计的风格<br>

## 迭代器
```c
typedef int type; //本库默认均已int 为默认类型

typedef struct
{
    type *data; //data 均是 指向数据的指针
} CTL_array_iterator;

CTL_array_iterator CTL_array_iterator_move(const CTL_array_iterator *handle, size_t pos, bool sub);

bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right);

ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right);

bool CTL_array_iterator_more(const CTL_array_iterator *left, const CTL_array_iterator *right);

```