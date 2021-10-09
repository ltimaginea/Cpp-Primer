#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <type_traits>

class Str
{
public:
	Str(std::string s = std::string()) :s_(s) {  }
	Str(const Str&) = default;
	Str(Str&& rs) :s_(std::move(rs.s_)) {  }	// 为达到测试目的，移动构造函数故意不标记为 noexcept
	Str& operator=(const Str&) = default;
	Str& operator=(Str&&) = default;
	~Str() = default;
private:
	std::string s_;
};

class T
{
public:
	T(std::string str = std::string()) :str_(str) {  }
	T(const T&) = default;
	T(T&&) = default;	// 成员 str_ 的移动构造函数没有承诺不会抛出异常，所以该类 T 合成的移动构造函数是 noexcept(false) ，即是可能抛出异常的
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
	~T() = default;
private:
	Str str_;
};

int main()
{
	// 分别输出 false	true
	std::cout << std::boolalpha << std::is_nothrow_move_constructible<T>::value << std::noboolalpha << std::endl;
	std::cout << std::boolalpha << std::is_nothrow_move_assignable<T>::value << std::noboolalpha << std::endl;

	T t1("11");
	T t2("22");
	T t3("33");
	T t4("44");

	std::vector<T> vc;
	vc.push_back(std::move(t1));
	vc.push_back(std::move(t2));
	vc.push_back(std::move(t3));
	vc.push_back(std::move(t4));

	return 0;
}