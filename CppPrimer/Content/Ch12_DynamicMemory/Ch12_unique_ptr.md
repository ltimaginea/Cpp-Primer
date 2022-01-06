# ***unique_ptr***

[unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) 提供专属所有权（exclusive ownership）的智能指针：当unique_ptr被销毁时，它指向的对象被释放。

初始化unique_ptr必须采用直接初始化形式。

```cpp
unique_ptr<int> p1(new int(42));
// make_unique: C++14起
unique_ptr<string> p2 = make_unique<string>(12, 'T');
```

unique_ptr 不支持普通的拷贝或赋值操作，但可以进行移动操作。unique_ptr的移动赋值，接管新资源的同时，还会销毁旧资源。

```cpp
// 错误，unique_ptr 不支持拷贝
// unique_ptr<string> p3(p2);
unique_ptr<string> p4 = make_unique<string>("TianZhou2");
// 错误，unique_ptr 不支持赋值
// p4 = p2;

unique_ptr<string> up1(new string("Good morning"));
unique_ptr<string> up2(make_unique<string>("sleep"));
unique_ptr<string> up3 = make_unique<string>("weak up");
unique_ptr<string> up4(std::move(up1));
// copy!
unique_ptr<string> up5(make_unique<string>(*up2));
// safe copy!
unique_ptr<string> up6(up2 ? make_unique<string>(*up2) : nullptr);

up1.reset(up2.release());
// 接管右侧新资源，释放左侧旧资源，可以认为等效于上一句
up3 = std::move(up4);
// copy!
up5 = make_unique<string>(*up3);	// make_unique的返回值是临时对象，即右值
// safe copy!
up5 = up3 ? make_unique<string>(*up3) : nullptr;
// up5.reset(make_unique<string>(*up3).release());		// 同上等效
// up5.reset(new string(*up3));		// 同上等效

// 不建议下面这样赋值，其安全性依赖于*up的类型的operator=的实现，安全性不稳定
// *up1 = *up5;

// We can use make_unique to create a unique_ptr to an array, but have to initialize the array separately.
// Create a unique_ptr to an array of 5 integers.
auto p = make_unique<int[]>(5);
// Initialize the array elements separately.
for (int i = 0; i < 5; ++i) { p[i] = i; }
```

虽然我们不能拷贝或赋值unique_ptr，但可以通过调用 `release` 或 `reset` 将指针的所有权从一个（非const）unique_ptr转移给另一个unique。

```cpp
unique_ptr<string> p5(p2.release());
// reset释放了p5原来指向的内存
p5.reset(p4.release());
```

`reset` 如果unique_ptr不为空，则释放unique_ptr指向的对象；如果提供了内置指针参数，令unique_ptr重新指向给定的指针，否则将其置为空。

`release` 放弃对指针的控制权，返回unique_ptr当前保存的指针并将其置为空。release不会释放内存，但是unique_ptr对象被解除了释放内存的责任， 其他实体必须负责在某个时候释放内存。

```cpp
// 错误，release()不会释放内存，而且我们丢失了指针
// p5.release();
// 正确，但必须记得释放内存
auto p = p5.release();
delete p;
```

不能拷贝unique_ptr的规则有一个例外：我们可以拷贝或赋值一个将要被销毁的unique_ptr。下面的程序，编译器知道函数要返回的对象将要被销毁。函数以值方式返回一个unique_ptr是合法的，因为unique_ptr存在一个有效定义的move constructor，返回值将会被移动构造，并且我们不需要也不应该 `std::move()` 返回的unique_ptr，因为这可能会对编译器的(N)RVO帮倒忙，详见 [Ch06_03_Return.md#return-a-non-reference-type](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch06_Functions/Ch06_03_Return.md#return-a-non-reference-type) 。

```cpp
unique_ptr<double> clone(double p)
{
	if (p > 0)
	{
		// 从函数返回一个unique_ptr
		return unique_ptr<double>(make_unique<double>(p));
	}
	else
	{
		// 也可以返回一个局部对象的拷贝
		auto up(make_unique<double>(p));
		return up;
	}
}
```

unique_ptr默认情况下用delete释放它指向的对象。我们可以重载一个unique_ptr中默认的删除器，重载一个unique_ptr中的删除器会影响到unique_ptr类型以及如何构造（或reset）该类型的对象，我们必须在尖括号中unique_ptr指向类型之后提供删除器类型。在创建或reset一个这种unique_ptr类型的对象时，必须提供一个指定类型的可调用对象（删除器）。

**unique_ptr自定义删除器需要单个unique_ptr内部指针类型的参数**。

```cpp
auto del = [](int* p) { delete p; cout << "deleter" << endl; };
// unique_ptr需要在模板参数中指明删除器类型，但shared_ptr不需要
unique_ptr<int, decltype(del)> uptr(new int(42), del);

shared_ptr<int> sptr(new int(42), del);
```

为了用一个unique_ptr管理动态数组，我们必须在对象类型后面跟一对空方括号[]，即 `unique_ptr<T[]>` 。类型说明符中的方括号指出unique_ptr指向一个T类型数组。由于unique_ptr指向一个数组，当unique_ptr销毁它管理的指针时，会自动使用delete[]。

当一个unique_ptr指向一个数组时，我们可以使用下标运算符来访问数组中的元素。但是，我们不能使用点和箭头成员运算符。毕竟unique_ptr指向的是一个数组而不是单个对象，因此这些运算符是无意义的。

 `unique_ptr<T[]>` 作为一种知识上的了解即可，因为 `std::array` ， `std::vector` ， `std::string` 这些更好用的数据容器几乎总是比原始数组更好。

```cpp
// up指向一个包含10个未初始化int的数组
unique_ptr<int[]> up(new int[10]);
for (size_t i = 0; i != 10; ++i)
{
	// 用下标运算符来访问数组中的元素
	up[i] = i;
}
// 自动用 delete[] 销毁其指针
up.reset();
```



## ***Tips***

- 绝对不要将同一块内存绑定到多个独立创建的unique_ptr上，否则会触发运行时错误："double freed or corruption"。shared_ptr同是。

-  `make_unique` 是C++14才加入标准库，在C++11中我们可以手动实现一个基础功能版 make_unique ，这个基础函数不支持数组和自定义删除器。从这个基础函数可以看出，make 函数把实参完美转发给构造函数并返回构造出的智能指针。

  ```cpp
  template <typename T, typename... Ts>
  std::unique_ptr<T> make_unique(Ts&&... args)
  {
  	return std::unique_ptr<T>{new T{ std::forward<Ts>(args)... }};
  }
  ```

  

> ## *References*
>
> [How to: Create and use unique_ptr instances | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/how-to-create-and-use-unique-ptr-instances?view=msvc-160)
>
> [ make_unique |  Microsoft Docs](https://docs.microsoft.com/en-us/cpp/standard-library/memory-functions?view=msvc-160#make_unique)
>
> 