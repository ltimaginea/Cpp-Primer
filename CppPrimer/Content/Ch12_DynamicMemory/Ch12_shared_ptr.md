# ***shared_ptr***

[shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) 提供共享所有权（shared ownership）的智能指针：对共享对象来说，当最后一个指向它的shared_ptr被销毁时会被释放。

默认初始化的智能指针中保存着一个空指针。解引用一个智能指针返回它指向的对象。

```cpp
shared_ptr<int> p1;
shared_ptr<list<int>> p2;
```

在条件判断中使用智能指针，效果就是检测它是否为空，若它指向一个对象，则为true。

```cpp
if (!p2)
		cout << "p2 is nullptr" << endl;
```

最安全的分配和使用动态内存的方法是调用一个名为 `make_shared` 的标准库函数。此函数在动态内存中分配一个对象并初始化它，返回指向此对象的shared_ptr。 **make_shared用其参数来构造给定类型的对象**。例如，调用make_shared\<string\>时传递的参数必须与string的某个构造函数相匹配，调用make_shared\<int\>时传递的参数必须能用来初始化一个int。

```cpp
// 42
shared_ptr<int> p5 = make_shared<int>(42);
// 9999999999
shared_ptr<string> p6 = make_shared<string>(10, '9');
// 0
auto p7 = make_shared<int>();
```

我们还可以用 `new` 返回的指针来初始化智能指针。接受指针参数的智能指针构造函数是 `explicit` 的，因此，我们不能将一个内置指针隐式转换为一个智能指针，必须使用 `直接初始化` 形式来初始化一个智能指针。

```cpp
// 正确，使用了直接初始化形式
shared_ptr<int> p3(new int(1024));
// 错误，必须使用直接初始化形式，使用 赋值号= 初始化一个变量，执行的是 拷贝初始化
// shared_ptr<int> p4 = new int(1024);
```

`reset` 会更新引用计数，如果需要的话，会释放指向的对象。若p是唯一指向其对象的shared_ptr，reset会释放此对象。若传递了可选的参数**普通指针**q，会令p指向 q指向的对象，否则会将p置为空。若还传递了参数d，将会调用d而不是delete来释放q。

```cpp
p.reset();
p.reset(q);
p.reset(q, d);
```

每个shared_ptr都有一个关联的计数器，通常称其为 `引用计数` （reference count）。一旦一个shared_ptr的引用计数变为0，它就会自动释放自己所管理的对象。

**shared_ptr保证线程安全，引用计数的递增和递减是原子操作**。

```cpp
auto q = make_shared<double>(3.14);
auto r = make_shared<double>(6.28);
// 给r赋值，令它指向另一个地址。递增q指向的对象的引用计数，递减r原来指向的对象的引用计数。r原来指向的对象已经没有引用者，会自动释放
r = q;
```

我们可以将智能指针绑定到一个指向其他类型的资源的指针上，但是为了这样做，必须提供自己的操作来替代delete。

**智能指针是异常安全的**。使用智能指针，即使程序块过早结束，智能指针类也能确保在内存不再需要时将其释放。函数的退出有两种可能，正常处理结束或者发生了异常，无论哪种情况，局部对象都会被销毁。在下面的程序中，sp是一个shared_ptr，因此sp销毁时会检查引用计数。在此例中，sp是指向这块内存的唯一指针，因此内存会被释放掉。

```cpp
void f()
{
	shared_ptr<int> sp(new int(42));
	// 这段代码抛出一个异常，且在f中未被捕获
}	// 在函数结束时 shared_ptr 自动释放内存
```

与之相对的，当发生异常时，我们直接管理的内存是不会自动释放的。在下面的程序中，如果在new和delete之间发生异常，且异常未在f中被捕获，则内存就永远不会被释放了。在函数f之外没有指针指向这块内存，因此就无法释放它了。

```cpp
void f()
{
	int* p = new int(42);
	// 这段代码抛出一个异常，且在f中未被捕获
	delete p;
}	// 内存泄漏
```

## *Tips*

- 应该尽量避免混合使用普通指针和智能指针，容易出现空悬指针（dangling pointer）或造成将同一块内存绑定到多个独立创建的shared_ptr上，触发运行时错误："double freed or corruption"。
- share_ptr在无用之后仍然保留的一种可能情况是，你将shared_ptr存放在一个容器中，随后重排了容器，从而不再需要某些元素。在这种情况下，你应该确保用erase删除那些不再需要的shared_ptr元素。