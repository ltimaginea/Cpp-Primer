# ***unique_ptr***

[unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) 提供独享所有权（exclusive ownership）的智能指针：当unique_ptr被销毁时，它指向的对象被释放。

初始化unique_ptr必须采用直接初始化形式。

```cpp
unique_ptr<int> p1(new int(42));
// make_unique: C++14起
unique_ptr<string> p2 = make_unique<string>(12, 'T');
```

unique_ptr 不支持普通的拷贝或赋值操作。

```cpp
// 错误，unique_ptr 不支持拷贝
// unique_ptr<string> p3(p2);
unique_ptr<string> p4 = make_unique<string>("TianZhou2");
// 错误，unique_ptr 不支持赋值
// p4 = p2;
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

不能拷贝unique_ptr的规则有一个例外：我们可以拷贝或赋值一个将要被销毁的unique_ptr。下面的程序，编译器都知道要返回的对象将要被销毁。

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