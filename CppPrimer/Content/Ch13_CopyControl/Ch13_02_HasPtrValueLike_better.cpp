﻿#include <iostream>
#include <string>
#include <utility>

// value-like implementation of HasPtr, each HasPtr has its own copy of the string to which ps_ points
class HasPtr
{
	friend void Swap(HasPtr&, HasPtr&);
public:
	// default constructor and constructor that takes a string
	HasPtr(const std::string& s = std::string()) : ps_(new std::string(s)), d_(3.14) {  }
	// copy constructor
	HasPtr(const HasPtr& rhp) : ps_(rhp.ps_ ? (new std::string(*rhp.ps_)) : nullptr), d_(rhp.d_) {  }
	// move constructor
	HasPtr(HasPtr&& rhp) noexcept : ps_(rhp.ps_), d_(rhp.d_) { rhp.ps_ = nullptr; }
	// copy assignment operator
	HasPtr& operator=(const HasPtr&);
	// move assignment operator
	HasPtr& operator=(HasPtr&&) noexcept;
	// destructor
	~HasPtr() { delete ps_; }

	// assignment operator 的另一种实现，采用了“拷贝/移动并交换”的技术。
	// 传值和传引用形式的operator= ，在调用时存在二义性，实践时类定义中需要选择其一。
	// assignment operator is both the copy- and move-assignment operator
	//HasPtr& operator=(HasPtr);

private:
	std::string* ps_;
	double d_;
};

// 既可以处理自赋值的情况，也还是异常安全的
// copy assignment operator
HasPtr& HasPtr::operator=(const HasPtr& rhp)
{
	// if rhp.ps_ is not nullptr, copy the underlying string
	std::string* newp = rhp.ps_ ? (new std::string(*rhp.ps_)) : nullptr;

	// free the old memory
	delete ps_;

	// copy data from rhp into this object
	ps_ = newp;
	d_ = rhp.d_;

	// return this object
	return *this;
}

// move assignment operator
HasPtr& HasPtr::operator=(HasPtr&& rhp) noexcept
{
	// direct test for self-assignment
	if (this != &rhp)
	{
		// free the old memory
		delete ps_;
		ps_ = rhp.ps_;
		d_ = rhp.d_;
		rhp.ps_ = nullptr;
	}
	return *this;
}

// 注意 rhp 是按值传递的，即“拷贝/移动并交换”的技术
// assignment operator is both the copy- and move-assignment operator
//HasPtr& HasPtr::operator=(HasPtr rhp)
//{
//	Swap(*this, rhp);
//	return *this;
//}

inline void Swap(HasPtr& lhp, HasPtr& rhp)
{
	// swap the pointers, not the string data
	std::swap(lhp.ps_, rhp.ps_);
	// swap the double members
	std::swap(lhp.d_, rhp.d_);
}

class Foo
{
	friend void Swap(Foo&, Foo&);
private:
	HasPtr h_;
	double d_;
};

inline void Swap(Foo& lfoo, Foo& rfoo)
{
	Swap(lfoo.h_, rfoo.h_);
	std::swap(lfoo.d_, rfoo.d_);
}

int main()
{
	HasPtr hp1("Jenny");
	HasPtr hp2("Hello");
	hp1 = hp1;
	HasPtr hp3(hp2);
	HasPtr hp4(std::move(hp1));
	hp1 = hp2;
	hp1 = std::move(hp4);
	HasPtr hp5(hp4);
	hp3 = hp4;
	hp4 = hp2;
	HasPtr hp6("Tired");
	HasPtr hp7("Reflection");
	hp6 = std::move(hp6);
	hp7 = hp6;
	Foo f1, f2;
	Swap(f1, f2);
	return 0;
}

// tips: 
//   1. move constructor 和 move assignment operator 应该是 noexcept
