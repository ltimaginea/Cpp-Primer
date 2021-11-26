#include <iostream>
#include <string>

class Base
{
public:
	Base() = default;
	Base(const Base&) = default;
	Base(Base&&) = default;
	Base& operator=(const Base&) = default;
	Base& operator=(Base&&) = default;
	virtual ~Base() = default;
public:
	Base(int i) : i_(i) {  }
	Base(int i, double d) : i_(i), d_(d) {  }
	Base(int i, double d, std::string s) : i_(i), d_(d), s_(s) {  }
private:
	int i_;
	double d_;
	std::string s_;
};

class Derived : public Base
{
public:
	using Base::Base;	// inheriting constructors

	Derived() = default;
	Derived(const Derived&) = default;
	Derived(Derived&&) = default;
	Derived& operator=(const Derived&) = default;
	Derived& operator=(Derived&&) = default;
	~Derived() override = default;
private:
	int value_;
};

int main()
{
	Base b;
	Derived d1(5);		// 派生类自己的数据成员 Derived::value_ 将被默认初始化
	Derived d2(5, 3.14);	// 派生类自己的数据成员 Derived::value_ 将被默认初始化
	Derived d3(5, 3.14, "yesterday's memory");	// 派生类自己的数据成员 Derived::value_ 将被默认初始化

	return 0;
}
