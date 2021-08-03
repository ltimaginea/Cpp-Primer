# HasPtrValueLike

通常，管理类外资源的类必须定义拷贝控制成员，这种类需要通过析构函数来释放对象所分配的资源。一旦一个类需要析构函数，那么它几乎肯定也需要一个拷贝构造函数和一个拷贝赋值运算符。 

为了定义这些成员，我们首先必须确定此类型对象的拷贝语义。一般来说，有两种选择：可以定义拷贝操作，使类的行为看起来像一个值或者像一个指针。 

类的行为像一个值，意味着它应该也有自己的状态，每个对象都应该拥有一份自己的拷贝。当我们拷贝一个像值的对象时，副本和原对象是完全独立的。改变副本不会对原对象有任何影响，反之亦然。

行为像指针的类则共享状态。当我们拷贝一个这种类的对象时，副本和原对象使用相同的底层数据。改变副本也会改变原对象，反之亦然。

我们的HasPtr类有两个成员，一个int和一个string指针。通常，类直接拷贝内置类型（不包括指针）成员；这些成员本身就是值，因此通常应该让它们的行为像值一样。我们如何拷贝指针成员决定了像HasPtr这样的类是具有类值行为还是类指针行为。

## 行为像值的类 *value-like*

为了实现类值行为，HasPtr需要：

- 定义一个拷贝构造函数，完成string的拷贝，而不是拷贝指针。
- 定义一个析构函数来释放string 。
- 定义一个拷贝赋值运算符来从右侧运算对象拷贝 string data ，并释放对象的旧内存。

除了定义拷贝控制成员，管理资源的类通常还需要定义一个名为swap的函数。

```cpp
#include <iostream>
#include <string>

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
	// copy assignment operator 的另一种实现，采用了“拷贝并交换”的技术（为了避免调用operator=存在二义性，实践时类定义中需要二者选择其一）
	HasPtr& operator=(HasPtr);
	// destructor
	~HasPtr() { delete ps_; }
private:
	string* ps_;
	int i_;
};

// 既可以处理自赋值的情况，也还是异常安全的
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

// 注意 rhs 是按值传递的，即“拷贝并交换”的技术
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





## 类值交换操作

