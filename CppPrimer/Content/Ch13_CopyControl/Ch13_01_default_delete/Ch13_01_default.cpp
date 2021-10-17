#include <iostream>
#include <string>

class T
{
public:
	T(std::string str) : str_(str) {  }
	T() = default;
	T(const T&);	// 仅声明
	T(T&&) = default;
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
	~T() = default;
private:
	std::string str_;
};

T::T(const T&) = default;	// 只对成员的类外定义使用 =default ，合成的成员将是非内联函数

int main()
{
	T t1("11");
	T t2("22");
	T t3("33");
	T t4("44");
	T t5(t1);
	T t6(std::move(t1));
	t1 = t2;
	t3 = std::move(t4);

	return 0;
}

// Tips:
//	1. 我们只能对编译器可以合成的默认构造函数或拷贝控制成员使用 =default
//	2. 当我们在类内用 =default 修饰成员的声明时，合成的函数将隐式地声明为内联的（就像任何其他类内声明的成员函数一样）。
//	如果我们不希望合成的成员是内联函数，应该只对成员的类外定义使用=default，就像对拷贝构造函数所做的那样。