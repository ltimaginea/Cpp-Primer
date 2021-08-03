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
	//HasPtr& operator=(const HasPtr&);
	// copy assignment operator 的另一种实现，采用了“拷贝并交换”的技术（为了避免调用operator=存在二义性，实践时类定义中需要二者选择其一）
	HasPtr& operator=(HasPtr);
	// destructor
	~HasPtr() { delete ps_; }
private:
	string* ps_;
	int i_;
};

// 既可以处理自赋值的情况，也还是异常安全的
//HasPtr& HasPtr::operator=(const HasPtr& rhs)
//{
//	// copy the underlying string
//	auto newp = new string(*rhs.ps_);
//	// free the old memory
//	delete ps_;
//	// copy data from rhs into this object
//	ps_ = newp;
//	i_ = rhs.i_;
//	// return this object
//	return *this;
//}

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

int main()
{
	HasPtr hp1;
	HasPtr hp2("hi, mom");
	HasPtr hp3("hi, daddddddddddddddddddddddddddddddddddddd");
	HasPtr hp4(hp3);

	hp1 = hp2;
	hp2 = hp3;
	hp3 = hp1;

	return 0;
}

// tips: 
//   1. 一个合格的 copy assignment operator 既需要可以处理自赋值的情况，也还应该是异常安全的