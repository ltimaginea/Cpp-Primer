#include <iostream>
#include <string>
#include <memory>

class B
{
public:
	B(int b = 0) : b_(b) {  }
	virtual ~B() = default;
	virtual void ClassSize() { std::cout << "sizeof(B) = " << sizeof(B) << std::endl; }
protected:
	// Make the polymorphic base class copy/move protected to prevent the slicing, 
	// and so that only derived classes can invoke them in their own copy/move.
	B(const B&) = default;
	B(B&&) = default;
	B& operator=(const B&) = default;
	B& operator=(B&&) = default;
private:
	int b_;
};

class D1 final : public B
{
public:
	D1(int b = 1, std::string str = std::string("default")) : B(b), str_(str) {  }
	D1(const D1&) = default;
	D1(D1&&) = default;
	D1& operator=(const D1&) = default;
	D1& operator=(D1&&) = default;
	~D1() override = default;
	void ClassSize() override { std::cout << "sizeof(D1) = " << sizeof(D1) << std::endl; }
private:
	std::string str_;
};

class D2 final : public B
{
public:
	D2(int b = 2, int i = 911) : B(b), ptr_(std::make_unique<int>(i)) {  }
	D2(const D2& other) : B(other), ptr_(other.ptr_ ? std::make_unique<int>(*other.ptr_) : nullptr) {  }
	D2(D2&&) = default;
	D2& operator=(const D2& other) { B::operator=(other); ptr_ = other.ptr_ ? std::make_unique<int>(*other.ptr_) : nullptr; return *this; }
	D2& operator=(D2&&) = default;
	~D2() override = default;
	void ClassSize() override { std::cout << "sizeof(D2) = " << sizeof(D2) << std::endl; }
private:
	std::unique_ptr<int> ptr_;
};

int main()
{
	std::unique_ptr<B> b_ptr1 = std::make_unique<D1>(111, "A class with a virtual function should have a virtual or protected destructor");
	std::unique_ptr<B> b_ptr2 = std::make_unique<D2>(222, 110);

	return 0;
}
