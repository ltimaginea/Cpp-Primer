#include <iostream>
#include <string>
#include <utility>
#include <memory>

// value-like implementation of HasPtr, each HasPtr has its own copy of the string to which ps_ points
class HasPtr
{
public:
	// default constructor and constructor that takes a string
	HasPtr(const std::string& s = std::string()) : ps_(std::make_unique<std::string>(s)), d_(3.14) {  }
	// copy constructor
	HasPtr(const HasPtr& rhp) : ps_(rhp.ps_ ? std::make_unique<std::string>(*rhp.ps_) : nullptr), d_(rhp.d_) {  }
	// move constructor (Best Practice: =default)
	HasPtr(HasPtr&& rhp) noexcept : ps_(std::move(rhp.ps_)), d_(rhp.d_) {  }
	// copy assignment operator
	HasPtr& operator=(const HasPtr&);
	// move assignment operator (Best Practice: =default)
	HasPtr& operator=(HasPtr&&) noexcept;
	// destructor
	~HasPtr() {  }

	// assignment operator 的另一种实现，采用了“拷贝/移动并交换”的技术。
	// 传值和传引用形式的operator= ，在调用时存在二义性，实践时类定义中需要选择其一。
	// assignment operator is both the copy- and move-assignment operator
	//HasPtr& operator=(HasPtr);

public:
	void Swap(HasPtr&);
private:
	std::unique_ptr<std::string> ps_;
	double d_;
};

// 既可以处理自赋值的情况，也还是异常安全的
// copy assignment operator
HasPtr& HasPtr::operator=(const HasPtr& rhp)
{
	ps_ = rhp.ps_ ? std::make_unique<std::string>(*rhp.ps_) : nullptr;
	d_ = rhp.d_;
	return *this;
}

// move assignment operator (Best Practice: =default)
HasPtr& HasPtr::operator=(HasPtr&& rhp) noexcept
{
	// 可以不用再进行自赋值判断，unique_ptr的operator=可以处理自赋值的情况
	ps_ = std::move(rhp.ps_);
	d_ = rhp.d_;
	return *this;
}

// 注意 rhp 是按值传递的，即“拷贝/移动并交换”的技术
// assignment operator is both the copy- and move-assignment operator
//HasPtr& HasPtr::operator=(HasPtr rhp)
//{
//	Swap(rhp);
//	return *this;
//}

void HasPtr::Swap(HasPtr& rhp)
{
	ps_.swap(rhp.ps_);
	std::swap(d_, rhp.d_);
}

class Foo
{
public:
	Foo(double d = 6.18) : d_(d) {  }
	void Swap(Foo&);
private:
	HasPtr h_;
	double d_;
};

void Foo::Swap(Foo& rfoo)
{
	h_.Swap(rfoo.h_);
	std::swap(d_, rfoo.d_);
}

int main()
{
	HasPtr hp1("grow up");
	hp1 = hp1;
	HasPtr hp2("doing steps");
	HasPtr hp3(hp1);
	HasPtr hp4(std::move(hp2));

	hp1 = hp2;
	hp2 = hp3;

	HasPtr hp5("yourself");
	HasPtr hp6("ourselves");
	hp5.Swap(hp6);
	hp5 = hp6;

	HasPtr hp7("summer");
	hp5 = std::move(hp7);
	HasPtr hp8(hp7);

	Foo f1(111), f2(222);
	f1.Swap(f2);

	return 0;
}

// tips: 
//   1. move constructor 和 move assignment operator 应该是 noexcept
