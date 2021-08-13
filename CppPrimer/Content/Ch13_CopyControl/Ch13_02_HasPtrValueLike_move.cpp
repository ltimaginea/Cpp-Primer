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
	//HasPtr& operator=(HasPtr);

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
//HasPtr& HasPtr::operator=(HasPtr rhs)
//{
//	Swap(*this, rhs);
//	return *this;
//}

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

int main()
{
	HasPtr hp1;
	HasPtr hp2("hi, mom");
	HasPtr hp3("hi, dadddddddddddddddddddddddddddddddd");
	HasPtr hp4(std::move(hp1));

	hp1 = hp2;
	hp2 = hp3;
	hp3 = hp1;
	hp1 = std::move(hp4);

	Foo f1, f2;
	Swap(f1, f2);
	return 0;
}

// tips: 
//   1. move constructor 和 move assignment operator 应该是 noexcept