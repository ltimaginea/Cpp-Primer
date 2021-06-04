# ***weak_ptr***

[weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr) 一种不控制所指向对象生存期的智能指针（弱指针），它指向由一个 `shared_ptr` 管理的对象。因为是弱共享，所以将一个 `weak_ptr` 绑定到一个shared_ptr不会改变shared_ptr的引用计数。一旦最后一个指向对象的shared_ptr被销毁，对象就会被释放。即使有weak_ptr指向对象，对象也还是会被释放。

weak_ptr 不是一种独立的智能指针，而是shared_ptr的一种扩充，它用shared_ptr初始化，共享对象但不改变引用计数。

```cpp
auto sp = make_shared<int>(42);
// wp弱共享sp,sp的引用计数未改变
weak_ptr<int> wp = sp;
```

weak_ptr 不能解引用，也不能用作条件判断，检查其是否为空。

```cpp
// 错误，不能解引用
// cout << *wp << endl;

// 错误，不能用作条件判断，检查其是否为空
// if (wp)
```

由于对象可能不存在，所以我们不能使用weak_ptr直接访问对象，而必须调用 `lock` 。lock 以原子操作检查weak_ptr指向的对象是否仍存在，如果存在，lock返回一个指向共享对象的shared_ptr，否则，返回一个空shared_ptr。

```cpp
if (auto p = wp.lock())
{
	cout << *p << endl;
}
```

`use_count` 返回管理该对象的shared_ptr对象数量。非线程安全。

`expired` 检查被引用的对象是否已delete，等价于若use_count为0，返回true，否则返回false。非线程安全。

`reset` 将弱指针置为空。

​     

> ## [***std::weak_ptr - cppreference.com***](https://zh.cppreference.com/w/cpp/memory/weak_ptr)
>
> `std::weak_ptr` 用来表达临时所有权的概念：当某个对象只有存在时才需要被访问，而且随时可能被他人删除时，可以使用 `std::weak_ptr` 来跟踪该对象。需要获得临时所有权时，则将其转换为 [std::shared_ptr](https://zh.cppreference.com/w/cpp/memory/shared_ptr)，**此时如果原来的 [std::shared_ptr](https://zh.cppreference.com/w/cpp/memory/shared_ptr) 被销毁，则该对象的生命期将被延长至这个临时的 [std::shared_ptr](https://zh.cppreference.com/w/cpp/memory/shared_ptr) 同样被销毁为止**。

