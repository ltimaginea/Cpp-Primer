# Better Practices

1. 下文为《C++ Primer》13.2.1节的示例代码和相关分析，但示例代码中缺少了对拷贝源对象的成员指针的有效性判断，经过完善，增加了有效性判断，完善后的程序见：(1) [Ch13_02_HasPtrValueLike_better.cpp](./Ch13_02_HasPtrValueLike_better.cpp) (2) [Ch13_02_HasPtrValueLike_unique_ptr_better.cpp](./Ch13_02_HasPtrValueLike_unique_ptr_better.cpp) 
2. swap 与 namespace 

# HasPtrValueLike

通常，管理类外资源的类必须定义拷贝控制成员，这种类需要通过析构函数来释放对象所分配的资源。一旦一个类需要析构函数，那么它几乎肯定也需要一个拷贝构造函数和一个拷贝赋值运算符。 

为了定义这些成员，我们首先必须确定此类型对象的拷贝语义。一般来说，有两种选择：可以定义拷贝操作，使类的行为看起来像一个值或者像一个指针。 

类的行为像一个值，意味着它应该也有自己的状态，每个对象都应该拥有一份自己的拷贝。当我们拷贝一个像值的对象时，副本和原对象是完全独立的。改变副本不会对原对象有任何影响，反之亦然。即其拷贝操作执行的是“深拷贝”（deep copy）。

行为像指针的类则共享状态。当我们拷贝一个这种类的对象时，副本和原对象使用相同的底层数据。改变副本也会改变原对象，反之亦然。即其拷贝操作执行的是“浅拷贝”（shallow copy）。

我们的HasPtr类有两个成员，一个int和一个string指针。通常，类直接拷贝内置类型（不包括指针）成员；这些成员本身就是值，因此通常应该让它们的行为像值一样。我们如何拷贝指针成员决定了像HasPtr这样的类是具有类值行为还是类指针行为。

## 行为像值的类 *value-like*

为了实现类值行为，HasPtr需要：

- 定义一个拷贝构造函数，完成 string data 的拷贝，而不是拷贝指针。
- 定义一个析构函数来释放 string 。
- 定义一个拷贝赋值运算符来从右侧运算对象拷贝 string data ，并释放对象的旧内存。

除了定义拷贝控制成员，管理资源的类通常还需要定义一个名为 swap 的函数。

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

**一个合格的 *copy assignment operator* 既应该正确处理自赋值的情况，还应该是异常安全的**。

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



### Warning item 01

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

如果 rhs 和本对象是同一个对象，`delete ps_` 会释放 `*this` 和 rhs 指向的 string 。接下来，当我们在 `new` 表达式中试图拷贝 `*rhs.ps_` 时，就会访问一个指向无效内存的指针，其行为和结果是未定义的。

这一版同时还不具备“异常安全性”，异常发生时左侧运算对象将不是置于一个有意义的状态。具体地说，如果 `new string(*rhs.ps_)` 抛出异常（不论是因为分配时内存不足或因为 std::string 的拷贝构造函数抛出异常）， `*this` 最终会持有一个指针指向一块被删除的 std::string 。这样的指针有害。我们无法安全地删除它们（详见 [Ch13_02_HasPtrValueLike_Case_NotExceptionSafe.md](./Ch13_02_HasPtrValueLike_Case_NotExceptionSafe.md) ），甚至无法安全地读取它们（解引用）。

### Warning item 02

当类中含有指针成员时， 我们实现 copy assignment operator 时，不建议对指针成员进行解引用赋值，这样的实现存在安全方面的缺陷。

```cpp
// 下面这样编写赋值运算符是存在缺陷的！
HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
	*ps_ = *rhs.ps_;
	i_ = rhs.i_;

	return *this;
}
```

存在的缺陷如下：

- 这样的实现缺少通用性，有没有问题完全依赖于 `*ps_` 对应类型 operator= 的实现。标准库 std::string 的 operator= 是有着良好实现的，示例那样的实现暂时是没有问题的，但如果 `*ps_` 不是 std::string 而是别的类型，比如某个浅拷贝实现的 operator= 的类型，那这样就会出问题了。
- 当指针成员 `ps_` 为无效指针时，这时对指针解引用就会发生未定义行为。比如， `x = std::move(y); y = z;` ，移后源对象 `y` 的指针成员 `ps_` 就会是 `nullptr` ，这时再为 `y` 赋予新值将是不安全的，即这时对指针 `ps_` 解引用就会发生未定义行为。所以，指针成员解引用赋值那样实现的 copy assignment operator ，无法应对处理的一种情形便是 **对移后源对象赋予新值** 。
- 不具备异常安全性。赋值操作可能抛异常，如 `*ps_ = *rhs.ps_;` 可能存在动态内存分配，则可能抛异常。当类含有多个指针成员时，从对第二个指针成员解引用赋值开始，如果某次解引用赋值时抛出了异常，异常发生时左侧运算对象将不是置于一个有意义的状态，即类对象没有保持原状，一部分成员已经被赋予了新值，另一部分成员还保存着旧值。



## 类值移动赋值运算符

对规范的移动赋值，期待其[令被移动对象遗留于合法状态](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)（即有完好类不变式的状态），且在自赋值时要么[不做任何事](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c65-make-move-assignment-safe-for-self-assignment)，要么至少遗留对象于合法状态，并以非 const 引用返回左操作数，而且为 `noexcept` 。

在移动操作之后，移后源对象必须保持有效的、可析构的状态，但是用户不能对其值进行任何假设。

除了将移后源对象置为析构安全的状态之外，移动操作还必须保证对象仍然是有效的。一般来说，对象有效就是指可以安全地为其赋予新值或者可以安全地使用而不依赖其当前值。另一方面，移动操作对移后源对象中留下的值没有任何要求。因此，我们的程序不应该依赖于移后源对象中的数据。比如对于这样的语句： `x = std::move(y); y = z;` ，程序也要能够正确处理。

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

交换两个类值HasPtr对象时，如果我们使用标准库定义的 std::swap ，为了交换两个对象需要进行一次拷贝和两次赋值。理论上，这些内存分配都是不必要的。我们更希望swap交换指针，而不是分配string的新复件。

与拷贝控制成员不同，swap并不是必要的。但是，对于分配了资源的类，定义swap可能是一种很重要的优化手段。

定义swap的类通常用swap来定义它们的赋值运算符。这些运算符使用了一种名为 拷贝/移动并交换(copy/move and swap) 的技术。这种技术将左侧运算对象与右侧运算对象的一个复件进行交换。

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

### 拷贝并交换

之所以与右侧运算对象的复件而不是本体（传左值引用）进行交换，因为 拷贝赋值操作 不应该改变右侧对象，与本体交换会导致改变右侧对象。

使用拷贝和交换的赋值运算符自动就是异常安全的，且能正确处理自赋值。

在这个版本的赋值运算符中，参数并不是一个引用，我们将右侧运算对象以传值方式传递给了赋值运算符。因此， rhs 是右侧运算对象的一个复件。参数传递时拷贝HasPtr的操作会分配该对象的 string 的一个新复件。 

在赋值运算符的函数体中，我们调用Swap来交换 rhs 和 `*this` 中的数据成员。这个调用将左侧运算对象中原来保存的指针存入 rhs 中，并将 rhs 中原来的指针存入 `*this` 中。因此，在swap调用之后， `*this` 中的指针成员将指向新分配的 string ——右侧运算对象中 string 的一个复件。

当赋值运算符结束时， rhs 被销毁，HasPtr的析构函数将执行。此析构函数 `delete rhs` 现在指向的内存，即，释放掉左侧运算对象中原来的内存。 

这个技术的有趣之处是它自动处理了自赋值情况且天然就是异常安全的。它通过在改变左侧运算对象之前拷贝右侧运算对象保证了自赋值的正确，这与我们在原来的赋值运算符中使用的方法是一致的。它保证异常安全的方法也与原来的赋值运算符实现一样。代码中唯一可能抛出异常的是拷贝构造函数中的 `new` 表达式。如果真发生了异常，它也会在我们改变左侧运算对象之前发生。

### 移动并交换

现在让我们重新观察赋值运算符。此运算符有一个非引用参数，这意味着此参数要进行拷贝初始化。**依赖于实参的类型，拷贝初始化要么使用拷贝构造函数，要么使用移动构造函数——左值被拷贝，右值被移动**。因此，单一的赋值运算符就实现了拷贝赋值运算符和移动赋值运算符两种功能。

不管使用的是拷贝构造函数还是移动构造函数，赋值运算符的函数体都swap两个运算对象的状态。交换HasPtr会交换两个对象的指针（及int）成员。在swap之后，rhs中的指针将指向原来左侧运算对象所拥有的string。当rhs离开其作用域时，这个string将被销毁。



## References

- [Assignment Operators, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/assignment-operators)
- [Move constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor)
- [operator overloading - cppreference.com](https://en.cppreference.com/w/cpp/language/operators)
- [Copy assignment operator - cppreference.com](https://en.cppreference.com/w/cpp/language/copy_assignment)
- [Move assignment operator - cppreference.com](https://en.cppreference.com/w/cpp/language/move_assignment)
- [How to: Define move constructors and move assignment operators (C++) | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-160)
- [令被移动对象遗留于合法状态](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)
- [C++ 拷贝赋值运算符的规范实现应该是什么样 ？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/480261834)
- [浅拷贝与深拷贝_ltimaginea的博客-CSDN博客](https://blog.csdn.net/sinat_43125576/article/details/108936177)

