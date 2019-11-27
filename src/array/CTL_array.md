# array
## 前言
  我个人实现array 和 STL中的array 略有不同, 它更像是一个 不支持动态扩展的vector<br>
  其实我实现的这个array, 主要为了方便统一的操作, 比如插入多个数据的时候, 我们可以统一使用迭代器作为参数类型<br>
  由于array实现十分简单, 我这里就不进行过多讲解了, 你可以通过阅读下面代码 了解本库风格
## 容器和迭代器定义
```c
typedef int type;

typedef struct
{
    type *data;
} CTL_array_iterator; //迭代器句柄

typedef struct
{
    CTL_array_iterator begin;
    CTL_array_iterator end;
} CTL_array; //容器句柄

void CTL_array_new(CTL_array *handle, type *begin, type *end); //新建

void CTL_array_clear(CTL_array *handle); //清空

void CTL_array_push_back(CTL_array *handle, type data); //尾端加入一个元素

void CTL_array_pop_back(CTL_array *handle); //删除尾端元素

void CTL_array_insert(CTL_array *handle, const CTL_array_iterator *iterator, type data); //插入

void CTL_array_erase(CTL_array *handle, const CTL_array_iterator *iterator); //删除

CTL_array_iterator CTL_array_at(const CTL_array *handle, size_t pos); //返回一个指向pos位置的迭代器

CTL_array_iterator CTL_array_iterator_move(const CTL_array_iterator *handle, size_t pos, bool sub);

bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right);

ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right);

bool CTL_array_iterator_more(const CTL_array_iterator *left, const CTL_array_iterator *right);
```
  在STL中array的迭代器, 就是一个普通的指针, 容器的begin指向目前使用空间开头, end目前使用空间结尾, 因为这个array是我参照vector精简实现的<br>
  ![begin和end 示意图](/img/array%E5%AE%B9%E5%99%A8%20begin%E5%92%8Cend%20%E7%A4%BA%E6%84%8F%E5%9B%BE.png?raw=true "begin和end 示意图")

## 容器和迭代器实现
  array实现上并没有什么难点, 所以这里就不进行讲解了
```c
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
```
