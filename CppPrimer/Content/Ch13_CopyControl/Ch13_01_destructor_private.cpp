#include <iostream>
#include <string>
#include <memory>

class T
{
	friend void FriendDestroy(T* ptr);
public:
	static void StaticDestroy(T* ptr)
	{
		delete ptr;
	}

	// Destroy() is `delete this`, which looks like very dangerous, not recommended. The more recommended is `static void StaticDestroy(T* ptr)`.
	void Destroy()
	{
		delete this;
	}
public:
	void InstanceInMemberFunction()
	{
		// ok: destructor 'T::~T()' can normally call within this context
		T t;
	}
public:
	T() = default;
	T(const T&) = default;
	T(T&&) = default;
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
private:
	~T() { std::cout << "destructor is called" << std::endl; }
private:
	std::string str_;
};

void FriendDestroy(T* ptr)
{
	delete ptr;
}

int main()
{
	// error: destructor 'T::~T()' is private within this context
	//T t1;

	// error: destructor 'T::~T()' is private within this context where '~unique_ptr()' delete pointer
	//auto t2 = std::make_unique<T>();

	// no delete pointer and the memory leak would occur
	auto t3 = new T();
	// error: destructor 'T::~T()' is private within this context
	//delete t3;

	auto t4 = new T();
	// Destroy() is `delete this`, which looks like very dangerous, not recommended. The more recommended is `static void StaticDestroy(T* ptr)`.
	t4->Destroy();
	t4 = nullptr;

	auto t5 = new T();
	t5->InstanceInMemberFunction();
	T::StaticDestroy(t5);
	t5 = nullptr;

	auto t6 = new T();
	FriendDestroy(t6);
	t6 = nullptr;

	return 0;
}