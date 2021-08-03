#include <iostream>
#include <string>

using std::string;

// value-like implementation of HasPtr, each HasPtr has its own copy of the string to which ps_ points
class HasPtr
{
public:
	// default constructor and constructor that takes a string
	HasPtr(const string& s = string()) :ps_(new string(s)), i_(0) {  }
	// copy constructor
	HasPtr(const HasPtr& p) :ps_(new string(*p.ps_)), i_(p.i_) {  }
	// copy assignment operator
	HasPtr& operator=(const HasPtr&);
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