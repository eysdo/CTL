# array
## 前言
  vector算是STL中最常用的一个容器了, vector的操作和array差不多 两者都是连续存储, 两者唯一的区别在于空间的灵活性. array是静态的一但创建完成是无法动态扩充的, vector创建完成后会根据, 我们的使用情况进行动态扩充<br>
  vector 关键技术就在于动态扩充上面, 当空间不足后vector会先分配一个更大的空间, 再将数据迁移至新空间, 释放掉旧空间, 何时分配空间 怎样迁移空间是决定vector性能的关键

## 容器和迭代器定义
```c
typedef int type;

typedef struct
{
    type *data;
} CTL_vector_iterator;

typedef struct
{
    CTL_vector_iterator begin;
    CTL_vector_iterator end;
    size_t size;
    size_t capacity;
} CTL_vector;

void CTL_vector_new(CTL_vector *handle, size_t size);

void CTL_vector_delete(CTL_vector *handle);

void CTL_vector_clear(CTL_vector *handle);

void CTL_vector_push_back(CTL_vector *handle, type data);

void CTL_vector_pop_back(CTL_vector *handle);

void CTL_vector_insert(CTL_vector *handle, const CTL_vector_iterator *iterator, type data);

void CTL_vector_erase(CTL_vector *handle, const CTL_vector_iterator *iterator);

CTL_vector_iterator CTL_vector_at(const CTL_vector *handle, size_t pos);

CTL_vector_iterator CTL_vector_iterator_move(const CTL_vector_iterator *handle, size_t pos, bool front);

bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

bool CTL_vector_iterator_more(const CTL_vector_iterator *left, const CTL_vector_iterator *right);
```
  迭代器就是一个普通的指针, 容器的begin指向目前使用空间开头, end目前使用空间结尾<br>
  在STL中还拥有一个end_of_storage指针 指向当前空间的结尾, 在这里被我用 capacity代替了, capacity当前空间可存放的元素数量<br>
  size当前存放的元素数量<br>
  这里放一张 vector扩充示意图<br>
  ![vector扩充示意图](/img/vector%E6%89%A9%E5%85%85%20%E7%A4%BA%E6%84%8F%E5%9B%BE.png?raw=true "vector扩充示意图")

## 容器和迭代器实现

__new delete clear实现__
```c
void CTL_vector_new(CTL_vector *handle, size_t size)
{
    handle->size = 0;
    handle->capacity = size;
    handle->begin.data = (type *)CTL_allocate(sizeof(type) * size);
    handle->end.data = handle->begin.data;
}

void CTL_vector_clear(CTL_vector *handle)
{
    handle->size = 0;
    handle->end.data = handle->begin.data;
}

void CTL_vector_delete(CTL_vector *handle)
{
    CTL_vector_clear(handle);
    CTL_deallocate(handle->begin.data, sizeof(type) * handle->capacity);
}
```
__push_back实现__
```c
void CTL_vector_push_back(CTL_vector *handle, type data)
{
    //空间 不足 扩充一倍
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)CTL_allocate(2 * handle->capacity * sizeof(type));
        memmove(ptr, handle->begin.data, handle->capacity * sizeof(type));
        CTL_deallocate(handle->begin.data, handle->capacity * sizeof(type));
        handle->begin.data = ptr;
        handle->capacity *= 2;
        handle->end.data = ptr + handle->size;
    }

    *handle->end.data++ = data;
    ++handle->size;
}
```
  push_back在尾端添加一个新的元素时, 会去先检查当前备用空间是否充足, 备用空间充足 直接在end位置赋值.<br>
  备用空间不足的时候, 先去构造一个新的空间 将原数据迁移至新的空间, 并释放掉原空间和调整begin和end迭代器, 在新的空间尾部赋值<br>
<font color=Red>
  注意这里的动态扩展并不是在原空间尾部增加空间, 因为无法保证原空间尾部还拥有可用内存, vector迭代器是一个普通指针, 所以当空间发生扩展的时候, 先前的所有迭代器会失效变成野指针<br>
</font>

__pop_back实现__
```c
void CTL_vector_pop_back(CTL_vector *handle)
{
    --handle->end.data;
    --handle->size;
}
```

__insert实现__
```c
void CTL_vector_insert(CTL_vector *handle, const CTL_vector_iterator *iterator, type data)
{
    //空间不足扩充操作
    if (handle->size == handle->capacity)
    {
        type *ptr = (type *)CTL_allocate(2 * handle->capacity * sizeof(type));
        //拷贝插入点前面的数据
        memmove(ptr, handle->begin.data, sizeof(type) * (iterator->data - handle->begin.data));
        //拷贝插入点后面的数据 并空出一个位置
        memmove(ptr + (iterator->data - handle->begin.data) + 1, iterator->data, sizeof(type) * (handle->end.data - iterator->data));
        CTL_deallocate(handle->begin.data, handle->capacity * sizeof(type));
        handle->capacity *= 2;
        //重新配置迭代器
        ptr[(iterator->data - handle->begin.data)] = data;
        handle->end.data = ptr + (handle->end.data - handle->begin.data);
        handle->begin.data = ptr;
    }
    else
    {
        memmove(iterator->data + 1, iterator->data, sizeof(type) * (handle->end.data - iterator->data));
        *iterator->data = data;
    }
    ++handle->end.data;
    ++handle->size;
}
```
insert也会检查备用空间, 备用空间充足 直接调用memmove移动数据, 然后在插入点赋值就行<br>
当备用空间不足的时候会分配新的空间, 将原空间数据拷贝到新空间里, 这里拷贝工作被分为两个步骤<br>
步骤1 拷贝插入点前的数据<br>
步骤2 空出一个位置 并拷贝插入点后面的数据, 对于插入多个数据的操作 这里就会空出多个位置<br>
拷贝工作完成后 进行相应的赋值操作<br>

__其他操作实现__
这些操作过于简单 就不再讲述了<br>
```c
void CTL_vector_erase(CTL_vector *handle, const CTL_vector_iterator *iterator)
{
    memmove(iterator->data, iterator->data + 1, sizeof(type) * (handle->end.data - iterator->data - 1));
    --handle->end.data;
    --handle->size;
}

CTL_vector_iterator CTL_vector_at(const CTL_vector *handle, size_t pos)
{
    CTL_vector_iterator result;
    result.data = handle->begin.data + pos;
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
    return left->data == right->data;
}

ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left->data - right->data;
}

bool CTL_vector_iterator_more(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left->data > right->data;
}
```
__总结__

vector容器对于随机访问速度非常快, 但是插入和删除就显得不行了, 因此使用vector的时候尽量减少插入和删除操作, 如果需要频繁插入 删除最好使用list容器.<br>
在进行插入操作的时候一旦发生扩展空间, 先前的全部迭代器都会失效, 变成野指针, 即便没有发生扩展操作, 迭代器所指对象可能也会发生变化<br>

参考文献<br>
---
[STL源码剖析](https://item.jd.com/11821611.html)<br>