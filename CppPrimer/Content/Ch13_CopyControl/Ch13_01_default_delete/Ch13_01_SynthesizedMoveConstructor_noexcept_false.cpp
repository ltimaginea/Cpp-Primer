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
	Str(Str&& rs) :s_(std::move(rs.s_)) {  }	// Ϊ�ﵽ����Ŀ�ģ��ƶ����캯�����ⲻ���Ϊ noexcept
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
	T(T&&) = default;	// ��Ա str_ ���ƶ����캯��û�г�ŵ�����׳��쳣�����Ը��� T �ϳɵ��ƶ����캯���� noexcept(false) �����ǿ����׳��쳣��
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
	~T() = default;
private:
	Str str_;
};

int main()
{
	// �ֱ���� false	true
	std::cout << std::boolalpha << std::is_nothrow_move_constructible<T>::value << std::noboolalpha << std::endl;
	std::cout << std::boolalpha << std::is_nothrow_move_assignable<T>::value << std::noboolalpha << std::endl;

	T t1("11");
	T t2("22");
	T t3("33");
	T t4("44");

	// ���� vector ֪��Ԫ�����͵��ƶ����캯�������׳��쳣�����������·����ڴ�Ĺ����У����ͱ���ʹ�ÿ������캯���������ƶ����캯�����ó����ʹ�ÿ������캯����
	std::vector<T> vc;
	vc.push_back(std::move(t1));
	vc.push_back(std::move(t2));
	vc.push_back(std::move(t3));
	vc.push_back(std::move(t4));

	return 0;
}