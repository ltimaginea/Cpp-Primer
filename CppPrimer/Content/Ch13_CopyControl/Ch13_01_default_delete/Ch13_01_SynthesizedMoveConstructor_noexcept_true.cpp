#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <type_traits>

class T
{
public:
	T(std::string str = std::string()) :up_(std::make_unique<std::string>(str)) {  }
	T(const T& rup) :up_(std::make_unique<std::string>(*rup.up_)) {  }
	T(T&&) = default;	// 成员 up_ 是 unique_ptr 类型，其移动构造函数是 noexcept 的，所以该类 T 的合成移动构造函数也会是 noexcept 的
	T& operator=(const T& rup) { up_ = std::make_unique<std::string>(*rup.up_); return *this; }
	T& operator=(T&&) = default;
	~T() = default;
private:
	std::unique_ptr<std::string> up_;
};

int main()
{
	// 分别输出 true		true
	std::cout << std::boolalpha << std::is_nothrow_move_constructible<T>::value << std::noboolalpha << std::endl;
	std::cout << std::boolalpha << std::is_nothrow_move_assignable<T>::value << std::noboolalpha << std::endl;

	T t1("11");
	T t2("22");
	T t3("33");
	T t4("44");

	// 除非 vector 知道元素类型的移动构造函数不会抛出异常，否则在重新分配内存的过程中，它就必须使用拷贝构造函数而不是移动构造函数。该程序会使用移动构造函数。
	std::vector<T> vc;
	vc.push_back(std::move(t1));
	vc.push_back(std::move(t2));
	vc.push_back(std::move(t3));
	vc.push_back(std::move(t4));

	return 0;
}