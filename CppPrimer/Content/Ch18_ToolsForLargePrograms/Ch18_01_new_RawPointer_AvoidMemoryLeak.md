# Raw pointer Avoid memory leak

程序使用 raw pointer 时，资源往往被显式管理（managed explicitly）。以此方式使用 raw pointer 的典型例子是，以 `new` 和 `delete` 创建和销毁对象：

```cpp
void Foo()
{
	Widget* ptr = new Widget;	// create an object explicitly

	// ... perform some operations(may throw exceptions)

	delete ptr;			// clean up(destroy the object explicitly)
}
```



这个函数将成为麻烦制造者。一个明显的问题是，有可能忘记 delete 对象，特别是如果你在函数中有个 return 语句。另一个较不明显的危险是**它可能抛出异常，那将立刻退离（exit）函数，末尾的 delete 语句也就没机会被调用，导致内存泄漏**，或更一般性地说，资源泄漏。 

为了避免如此的资源泄漏，通常函数会捕捉所有异常，例如：完整程序见 [Ch18_01_new_RawPointer_AvoidMemoryLeak.cpp](./Ch18_01_new_RawPointer_AvoidMemoryLeak.cpp) 

```cpp
void Foo()
{
	Widget* ptr = new Widget;

	try
	{
		// perform some operations...(may throw exceptions)
		std::string().at(1);	// this generates an std::out_of_range
	}
	catch (...)			// for any exception
	{
		delete ptr;		// clean up
		throw;			// rethrow the exception
	}

	delete ptr;			// clean up on normal end
}
```



为了在异常发生时能够适当处理好对象的删除，代码变得比较复杂，而且累赘。如果第二个对象也以此方式处理，或需要一个以上的 `catch` 子句，情况会变得更糟。这是一种不好的编程风格，应该避免，因为它复杂而又容易出错。 

对此， smart pointer 可以带来帮助。 Smart pointer 可以在它自身被销毁时释放其所指向的数据——不论是正常结束或异常结束。现代 C++ 程序应该优先使用智能指针。

