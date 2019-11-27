# CTL
## 前言
  该分支主要是分析 SIG STL 源码<br>
  由于C语并没有模板和重载 所以 我只是实现了部分STL特性, 另外该分支为了增加可读性 减少了宏使用, 部分宏实现的特性请参照主分支<br>
  本库 大量参考 侯捷老师的 [STL源码剖析](https://item.jd.com/11821611.html)<br>
  特别感谢 [GitHub: FunctionDou SIG STL源码分析](https://github.com/FunctionDou/STL)<br>
  同时 也感谢网络当中的各位大佬们 在我学习过程 你们的博客 文章 让我少走了许多弯路<br>
  由于本人能力有限 分析时可能出现一些错误 也欢迎各位大佬们 指出错误<br>
  
STL 分为 以下 六大模块
-
  * [空间分配器](/src/allocator/CTL_allocator.md#空间分配器)
  * [迭代器](/src/iterator/CTL_iterator.md#迭代器)
  * 容器
    * 序列容器
      * [array](/src/array/CTL_array.md#array)
      * [vector](/src/vector/CTL_vector.md#vector)
    * 关联容器
  * 算法
  * 仿函数
  * 配接器
