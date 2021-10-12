#include <iostream>
#include <cassert>

class Singleton
{
public:
	static Singleton& Instance()
	{
		static Singleton instance;
		return instance;
	}
public:
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;
private:
	Singleton() = default;
	~Singleton() = default;
};

int main()
{
	auto& a = Singleton::Instance();
	auto& b = Singleton::Instance();
	assert(&a == &b);

	return 0;
}