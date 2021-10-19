# destructor

析构函数执行与构造函数相反的操作：构造函数初始化对象的非 `static` 数据成员，还可能做一些其他工作；析构函数释放对象使用的资源，并销毁对象的非 `static` 数据成员。 

当一个类未定义自己的析构函数时，编译器会为它定义一个合成析构函数（synthesized destructor），合成析构函数的函数体为空。合成析构函数具有 `public` 访问层级且是 `inline` 的。

析构函数是类的一个成员函数，名字由波浪号接类名构成。它没有返回值，也不接受参数。

由于析构函数不接受参数，因此它不能被重载。对一个给定类，只会有唯一一个析构函数。

```cpp
class T
{
public:
	T(const std::string& str = std::string()) : ps_(new std::string(str)) {  }
	~T() { delete ps_; }
	//...
private:
	std::string* ps_;
};
```

## 析构函数完成什么工作

如同构造函数有一个初始化部分和一个函数体，析构函数也有一个函数体和一个析构部分。在一个构造函数中，成员的初始化是在函数体执行之前完成的，且按照它们在类中出现的顺序进行初始化。**在一个析构函数中，首先执行函数体，然后销毁成员。成员按初始化顺序的逆序销毁**。 

**在对象最后一次使用之后，析构函数的函数体可执行类设计者希望执行的任何收尾工作。通常，析构函数释放对象在生存期分配的所有资源**。 

**在一个析构函数中，不存在类似构造函数中初始化列表的东西来控制成员如何销毁，析构部分是隐式的。成员销毁时发生什么完全依赖于成员的类型。销毁类类型的成员需要执行成员自己的析构函数。内置类型没有析构函数，因此销毁内置类型成员什么也不需要做**。

认识到析构函数体自身并不直接销毁成员是非常重要的。成员是在析构函数体之后隐含的析构阶段中被销毁的。在整个对象销毁过程中，析构函数体是作为成员销毁步骤之外的另一部分而进行的。

当指向一个对象的引用或指针离开作用域时，析构函数不会执行。隐式销毁一个内置指针类型的成员不会 `delete` 它所指向的对象。与内置指针不同，智能指针是类类型，所以具有析构函数。因此，与内置指针不同，智能指针成员在析构阶段会被自动销毁。

## 什么时候会调用析构函数

无论何时一个对象被销毁，就会自动调用其析构函数：

- 变量在离开其作用域时被销毁。
- 当一个对象被销毁时，其成员被销毁。
- 容器（无论是标准库容器还是数组）被销毁时，其元素被销毁。
- 对于动态分配的对象，当对指向它的指针应用 `delete` 运算符时被销毁。
- 对于临时对象，当创建它的完整表达式结束时被销毁。 

由于析构函数自动运行，我们的程序可以按需要分配资源，而（通常）无须担心何时释放这些资源。

## deleted destructor

值得注意的是，我们不能删除析构函数。如果析构函数被删除，就无法销毁此类型的对象了。对于一个删除了析构函数的类型，编译器将不允许定义该类型的变量或创建该类的临时对象。而且，如果一个类有某个成员的类型删除了析构函数，我们也不能定义该类的变量或临时对象。因为如果一个成员的析构函数是删除的，则该成员无法被销毁。而如果一个成员无法被销毁，则对象整体也就无法被销毁了。

对于删除了析构函数的类型，虽然我们不能定义这种类型的变量或成员，但可以动态分配这种类型的对象。但是，不能释放这些对象。

```cpp
#include <iostream>
#include <string>
#include <memory>

class T
{
public:
	~T() = delete;
public:
	T() = default;
	T(const T&) = default;
	T(T&&) = default;
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
private:
	std::string str_;
};

int main()
{
	// error: use of deleted destructor function 'T::~T()'
	//T t1;

	// error: use of deleted destructor function 'T::~T()'
	//auto t2 = std::make_unique<T>();

	// no delete pointer and the memory leak would occur
	auto t3 = new T();
	// error: use of deleted destructor function 'T::~T()'
	//delete t3;

	return 0;
}
```

## private destructor

析构函数是 `private` 时，那么当对象销毁时或对于动态分配的对象，当对指向它的指针应用 `delete` 运算符时，只有销毁的对象所处的上下文环境为成员函数体或友元函数体内部时，才能够正常调用 `private` 析构函数，才能合法销毁对象。其原因是，只有成员函数和友元才能访问类的私有成员。

```cpp
#include <iostream>
#include <string>
#include <memory>

class T
{
	friend void FriendDestroy(T* ptr);
public:
	static void StaticDestroy(T* ptr)
	{
		delete ptr;
	}

	// Destroy() is 'delete this', which looks like very dangerous, not recommended. The more recommended is 'static void StaticDestroy(T* ptr)'.
	void Destroy()
	{
		delete this;
	}
public:
	void InstanceInMemberFunction()
	{
		// ok: destructor 'T::~T()' can normally call within this context
		T t;
	}
public:
	T() = default;
	T(const T&) = default;
	T(T&&) = default;
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
private:
	~T() { std::cout << "destructor is called" << std::endl; }
private:
	std::string str_;
};

void FriendDestroy(T* ptr)
{
	delete ptr;
}

int main()
{
	// error: destructor 'T::~T()' is private within this context
	//T t1;

	// error: destructor 'T::~T()' is private within this context where '~unique_ptr()' delete pointer
	//auto t2 = std::make_unique<T>();

	// no delete pointer and the memory leak would occur
	auto t3 = new T();
	// error: destructor 'T::~T()' is private within this context
	//delete t3;

	auto t4 = new T();
	// Destroy() is 'delete this', which looks like very dangerous, not recommended. The more recommended is 'static void StaticDestroy(T* ptr)'.
	t4->Destroy();
	t4 = nullptr;

	auto t5 = new T();
	t5->InstanceInMemberFunction();
	T::StaticDestroy(t5);
	t5 = nullptr;

	auto t6 = new T();
	FriendDestroy(t6);
	t6 = nullptr;

	return 0;
}
```

