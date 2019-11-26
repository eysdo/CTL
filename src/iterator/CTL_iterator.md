# 迭代器
## 前言
  迭代器是一种抽象设计概念, 它提供一种方法, 可以让我们依序遍历容器中的各个元素, 却又不需要过多了解容器内部实现细节<br>
  我们可以将容器理解为交通工具, 迭代器就是操作这些交通工具的司机, 当我们前往一个地方的时候, 仅需要将目的地告诉司机,
  并不需要过多了解交通工具的操作细节<br>
  迭代器将容器和对容器的操作(算法), 进行了分离, 算法通过迭代器操作容器，只要算法使用迭代器的通用接口, 便可以在少量或无需修改源码的情况下, 支持另一种容器<br>
  <br>
  由于C语言并没有模板和运算符重载等操作, 所以我只是实现的少量迭代器操作<br>
  如果你想深入了解迭代器, 可以参考以下文献<br>
  [STL源码剖析](https://item.jd.com/11821611.html)<br>
  [GitHub: FunctionDou SIG STL源码分析](https://github.com/FunctionDou/STL)<br>

## 迭代器简介
  迭代器名称 | 支持的读写操作 | 本库是否实现
  ----------|---------------|-------------|
  input iterator(输入迭代器) | 只读 | 并没有单独的实现
  output iterator(输出迭代器) | 只写 | 并没有单独的实现
  forward iterator(正向迭代器) | 读和写 | 并没有单独的实现
  bidirectional iterator(双向迭代器) | 读和写 | 已实现
  random access iterator(随即访问迭代器) | 读和写 | 已实现, 但运算符操作不完整

  迭代器名称 | *和-> | ++i和i++ | --i和i-- | ==和!= | 其他运算符
  --------------------------|---|---|---|---|---
  input iterator(输入迭代器) | √ | √ | × | √ | × 
  output iterator(输出迭代器)| √ | √ | × | × | × 
  forward iterator(正向迭代器)| √ | √ | × | √ | × 
  bidirectional iterator(双向迭代器)| √ | √ | √ | √ | × 
  random access iterator(随即访问迭代器)| √ | √ | √ | √ | √ 

## 已实现的迭代器运算操作
```c
//移动操作
iterator CTL_iterator_move(iterator handle, size_t pos, bool sub);

//left == right
bool CTL_iterator_equal(iterator left, iterator right);

//left - right 计算两个容器的距离
bool CTL_iterator_diff(iterator left, iterator right);

//left > right
bool CTL_iterator_more(iterator left, iterator right);
```