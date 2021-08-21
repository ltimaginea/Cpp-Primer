#include <iostream>
#include <string>
#include <utility>
#include <memory>

// value-like implementation of HasPtr, each HasPtr has its own copy of the string to which ps_ points
class HasPtr
{
public:
	// default constructor and constructor that takes a string
	HasPtr(const std::string& s = std::string()) :ps_(std::make_unique<std::string>(s)), i_(0) {  }

	// copy constructor
	HasPtr(const HasPtr& p) :ps_(std::make_unique<std::string>(*p.ps_)), i_(p.i_) {  }
	// copy assignment operator
	HasPtr& operator=(const HasPtr&);

	// move constructor
	HasPtr(HasPtr&& p) noexcept :ps_(std::move(p.ps_)), i_(p.i_) {  }
	// move assignment operator
	HasPtr& operator=(HasPtr&&) noexcept;

	// assignment operator 的另一种实现，采用了“拷贝/移动并交换”的技术。
	// 传值和传引用形式的operator= ，在调用时存在二义性，实践时类定义中需要选择其一。
	// assignment operator is both the copy- and move-assignment operator
	//HasPtr& operator=(HasPtr);

	// destructor
	~HasPtr() {  }
public:
	void Swap(HasPtr&);
private:
	std::unique_ptr<std::string> ps_;
	int i_;
};

// 既可以处理自赋值的情况，也还是异常安全的
// copy assignment operator
HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
	ps_ = std::make_unique<std::string>(*rhs.ps_);
	i_ = rhs.i_;
	return *this;
}

// move assignment operator
HasPtr& HasPtr::operator=(HasPtr&& rhs) noexcept
{
	// 可以不用再进行自赋值判断，unique_ptr的operator=可以处理自赋值的情况
	//if (this != &rhs)
	{
		ps_ = std::move(rhs.ps_);
		i_ = rhs.i_;
	}
	return *this;
}

// 注意 rhs 是按值传递的，即“拷贝/移动并交换”的技术
// assignment operator is both the copy- and move-assignment operator
//HasPtr& HasPtr::operator=(HasPtr rhs)
//{
//	Swap(rhs);
//	return *this;
//}

void HasPtr::Swap(HasPtr& rhs)
{
	ps_.swap(rhs.ps_);
	std::swap(i_, rhs.i_);
}

class Foo
{
private:
	HasPtr h_;
	int i_;
public:
	Foo(int i = 0) :i_(i) {  }
public:
	void Swap(Foo&);
};

void Foo::Swap(Foo& rhs)
{
	h_.Swap(rhs.h_);
	std::swap(i_, rhs.i_);
}

int main()
{
	HasPtr hp1;
	HasPtr hp2("hi, mom");
	HasPtr hp3("hi, dadddddddddddddddddddddddddddddddd");
	HasPtr hp4(hp2);
	HasPtr hp5(std::move(hp3));
	hp3 = HasPtr("mmmmmmmmmmmmmmmmmmmmmmma");
	hp2 = hp2;
	hp3 = std::move(hp3);

	Foo f1(111), f2(222);
	f1.Swap(f2);

	return 0;
}

// tips: 
//   1. move constructor 和 move assignment operator 应该是 noexcept