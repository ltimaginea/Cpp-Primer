#include <iostream>
#include <string>
#include <memory>

class T
{
public:
	~T() = delete;
public:
	T() = default;
	T(const T&) = default;
	T(T&&) = default;
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
private:
	std::string str_;
};

int main()
{
	// error: use of deleted destructor function 'T::~T()'
	//T t1;

	// error: use of deleted destructor function 'T::~T()'
	//auto t2 = std::make_unique<T>();

	// no delete pointer and the memory leak would occur
	auto t3 = new T();
	// error: use of deleted destructor function 'T::~T()'
	//delete t3;

	return 0;
}