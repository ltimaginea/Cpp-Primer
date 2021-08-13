# HasPtrValueLike

通常，管理类外资源的类必须定义拷贝控制成员，这种类需要通过析构函数来释放对象所分配的资源。一旦一个类需要析构函数，那么它几乎肯定也需要一个拷贝构造函数和一个拷贝赋值运算符。 

为了定义这些成员，我们首先必须确定此类型对象的拷贝语义。一般来说，有两种选择：可以定义拷贝操作，使类的行为看起来像一个值或者像一个指针。 

类的行为像一个值，意味着它应该也有自己的状态，每个对象都应该拥有一份自己的拷贝。当我们拷贝一个像值的对象时，副本和原对象是完全独立的。改变副本不会对原对象有任何影响，反之亦然。

行为像指针的类则共享状态。当我们拷贝一个这种类的对象时，副本和原对象使用相同的底层数据。改变副本也会改变原对象，反之亦然。

我们的HasPtr类有两个成员，一个int和一个string指针。通常，类直接拷贝内置类型（不包括指针）成员；这些成员本身就是值，因此通常应该让它们的行为像值一样。我们如何拷贝指针成员决定了像HasPtr这样的类是具有类值行为还是类指针行为。

## 行为像值的类 *value-like*

为了实现类值行为，HasPtr需要：

- 定义一个拷贝构造函数，完成 string data 的拷贝，而不是拷贝指针。
- 定义一个析构函数来释放string 。
- 定义一个拷贝赋值运算符来从右侧运算对象拷贝 string data ，并释放对象的旧内存。

除了定义拷贝控制成员，管理资源的类通常还需要定义一个名为swap的函数。

```cpp
#include <iostream>
#include <string>
#include <utility>

using std::string;

// value-like implementation of HasPtr, each HasPtr has its own copy of the string to which ps_ points
class HasPtr
{
	friend void Swap(HasPtr&, HasPtr&);
public:
	// default constructor and constructor that takes a string
	HasPtr(const string& s = string()) :ps_(new string(s)), i_(0) {  }

	// copy constructor
	HasPtr(const HasPtr& p) :ps_(new string(*p.ps_)), i_(p.i_) {  }
	// copy assignment operator
	HasPtr& operator=(const HasPtr&);

	// move constructor
	HasPtr(HasPtr&& p) noexcept :ps_(p.ps_), i_(p.i_) { p.ps_ = nullptr; }
	// move assignment operator
	HasPtr& operator=(HasPtr&&) noexcept;

	// assignment operator 的另一种实现，采用了“拷贝/移动并交换”的技术。
	// 传值和传引用形式的operator= ，在调用时存在二义性，实践时类定义中需要选择其一。
	// assignment operator is both the copy- and move-assignment operator
	HasPtr& operator=(HasPtr);

	// destructor
	~HasPtr() { delete ps_; }
private:
	string* ps_;
	int i_;
};

// 既可以处理自赋值的情况，也还是异常安全的
// copy assignment operator
HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
	// copy the underlying string
	auto newp = new string(*rhs.ps_);
	// free the old memory
	delete ps_;
	// copy data from rhs into this object
	ps_ = newp;
	i_ = rhs.i_;
	// return this object
	return *this;
}

// move assignment operator
HasPtr& HasPtr::operator=(HasPtr&& rhs) noexcept
{
	// direct test for self-assignment
	if (this != &rhs)
	{
		// free the old memory
		delete ps_;
		ps_ = rhs.ps_;
		i_ = rhs.i_;
		rhs.ps_ = nullptr;
	}
	return *this;
}

// 注意 rhs 是按值传递的，即“拷贝/移动并交换”的技术
// assignment operator is both the copy- and move-assignment operator
HasPtr& HasPtr::operator=(HasPtr rhs)
{
	Swap(*this, rhs);
	return *this;
}

inline void Swap(HasPtr& lhs, HasPtr& rhs)
{
	// swap the pointers, not the string data
	std::swap(lhs.ps_, rhs.ps_);
	// swap the int members
	std::swap(lhs.i_, rhs.i_);
}

class Foo
{
	friend void Swap(Foo&, Foo&);
private:
	HasPtr h_;
	int i_;
};

inline void Swap(Foo& lhs, Foo& rhs)
{
	Swap(lhs.h_, rhs.h_);
	std::swap(lhs.i_, rhs.i_);
}
```



## 类值拷贝赋值运算符

**当我们编写一个赋值运算符时，一个好的模式是先将右侧运算对象拷贝到一个局部临时对象中。当拷贝完成后，销毁左侧运算对象的现有成员就是安全的了。一旦左侧运算对象的资源被销毁，就只剩下将数据从临时对象拷贝到左侧运算对象的成员中了**。

**一个合格的 *copy assignment operator* 既需要可以处理自赋值的情况，也还应该是异常安全的**。

赋值运算符通常组合了构造函数和析构函数的操作。类似拷贝构造函数，赋值操作会从右侧运算对象拷贝数据。类似析构函数，赋值操作会销毁左侧运算对象的资源。但是，非常重要的一点是，这些操作是以正确的顺序执行的，即使将一个对象赋予它自身，也保证正确。而且，我们编写的赋值运算符还应该是异常安全的——当异常发生时能将左侧运算对象置于一个有意义的状态。

在本例中，通过先拷贝右侧运算对象的资源，我们可以处理自赋值情况，在完成拷贝后，我们释放左侧运算对象的资源，并更新指针指向新分配的 string ，以及从 rhs 拷贝 int 值到本对象。本例同时能保证在异常发生时代码也是安全的，即如果 `new string(*rhs.ps_)`  抛出异常， `ps_` （及其栖身的那个 HasPtr 对象）会保持原状。

```cpp
// 既可以处理自赋值的情况，也还是异常安全的
// copy assignment operator
HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
	// copy the underlying string
	auto newp = new string(*rhs.ps_);
	// free the old memory
	delete ps_;
	// copy data from rhs into this object
	ps_ = newp;
	i_ = rhs.i_;
	// return this object
	return *this;
}
```

为了说明防范自赋值操作的重要性，考虑如果赋值运算符如下编写将会发生什么，即如果把正确示例中的 拷贝右侧运算对象的资源 和 释放左侧运算对象的资源 的语句顺序调换，将会导致 无法处理自赋值和不具备“异常安全性”。

```cpp
// 下面这样编写赋值运算符是错误的！
HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
	// free the old memory
	delete ps_;
	// 如果rhs和*this是同一个对象，我们就将从已释放的内存中拷贝数据！
	// copy the underlying string
	auto newp = new string(*rhs.ps_);
	// copy data from rhs into this object
	ps_ = newp;
	i_ = rhs.i_;
	// return this object
	return *this;
}
```

如果 rhs 和本对象是同一个对象，delete ps_ 会释放 *this 和 rhs 指向的 string 。接下来，当我们在 new 表达式中试图拷贝 `*rhs.ps_` 时，就会访问一个指向无效内存的指针，其行为和结果是未定义的。

这一版同时还不具备“异常安全性”，异常发生时左侧运算对象将不是置于一个有意义的状态。具体地说，如果 `new string(*rhs.ps_)` 抛出异常（不论是因为分配时内存不足或因为 std::string 的拷贝构造函数抛出异常）， *this 最终会持有一个指针指向一块被删除的 std::string 。这样的指针有害。我们无法安全地删除它们，甚至无法安全地读取它们（解引用）。

## 类值移动赋值运算符

对规范的移动赋值，期待其令被移动对象遗留于合法状态（即有完好类不变式的状态），且在自赋值时要么不做任何事，要么至少遗留对象于合法状态，并以非 const 引用返回左操作数，而且为 `noexcept` 。

```cpp
// move assignment operator
HasPtr& HasPtr::operator=(HasPtr&& rhs) noexcept
{
	// direct test for self-assignment
	if (this != &rhs)
	{
		// free the old memory
		delete ps_;
		ps_ = rhs.ps_;
		i_ = rhs.i_;
		rhs.ps_ = nullptr;
	}
	return *this;
}
```



## 类值交换操作

交换两个类值HasPtr对象时，如果我们使用标准库定义的std::swap，为了交换两个对象需要进行一次拷贝和两次赋值。理论上，这些内存分配都是不必要的。我们更希望swap交换指针，而不是分配string的新副本。

与拷贝控制成员不同，swap并不是必要的。但是，对于分配了资源的类，定义swap可能是一种很重要的优化手段。

定义swap的类通常用swap来定义它们的赋值运算符。这些运算符使用了一种名为拷贝并交换（copy and swap）的技术。这种技术将左侧运算对象与右侧运算对象的一个副本进行交换。之所以与右侧运算对象的副本而不是本体进行交换，因为拷贝赋值操作不应该改变右侧对象，与本体交换会导致改变右侧对象。

使用拷贝和交换的赋值运算符自动就是异常安全的，且能正确处理自赋值。

```cpp
// 注意 rhs 是按值传递的，即“拷贝/移动并交换”的技术
// assignment operator is both the copy- and move-assignment operator
HasPtr& HasPtr::operator=(HasPtr rhs)
{
	Swap(*this, rhs);
	return *this;
}

inline void Swap(HasPtr& lhs, HasPtr& rhs)
{
	// swap the pointers, not the string data
	std::swap(lhs.ps_, rhs.ps_);
	// swap the int members
	std::swap(lhs.i_, rhs.i_);
}
```

在这个版本的赋值运算符中，参数并不是一个引用，我们将右侧运算对象以传值方式传递给了赋值运算符。因此，rhs是右侧运算对象的一个副本。参数传递时拷贝HasPtr的操作会分配该对象的string的一个新副本。 

在赋值运算符的函数体中，我们调用Swap来交换rhs和 *this 中的数据成员。这个调用将左侧运算对象中原来保存的指针存入rhs中，并将rhs中原来的指针存入 *this 中。因此，在swap调用之后， *this 中的指针成员将指向新分配的string——右侧运算对象中string的一个副本。

当赋值运算符结束时，rhs被销毁，HasPtr的析构函数将执行。此析构函数 delete rhs现在指向的内存，即，释放掉左侧运算对象中原来的内存。 

这个技术的有趣之处是它自动处理了自赋值情况且天然就是异常安全的。它通过在改变左侧运算对象之前拷贝右侧运算对象保证了自赋值的正确，这与我们在原来的赋值运算符中使用的方法是一致的。它保证异常安全的方法也与原来的赋值运算符实现一样。代码中唯一可能抛出异常的是拷贝构造函数中的new表达式。如果真发生了异常，它也会在我们改变左侧运算对象之前发生。



> ## *References*
>
> 1. [Move constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor)
> 2. [operator overloading - cppreference.com](https://en.cppreference.com/w/cpp/language/operators)
> 3. [Copy assignment operator - cppreference.com](https://en.cppreference.com/w/cpp/language/copy_assignment)
> 4. [Move assignment operator - cppreference.com](https://en.cppreference.com/w/cpp/language/move_assignment)
> 5. [How to: Define move constructors and move assignment operators (C++) | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-160)
>
> 
