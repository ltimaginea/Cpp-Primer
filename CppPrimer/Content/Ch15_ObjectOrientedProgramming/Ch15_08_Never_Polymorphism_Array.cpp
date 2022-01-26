#include <iostream>
#include <string>

class Base
{
public:
	virtual ~Base() = default;
	int b_ = 1;
};

class Derived : public Base
{
public:
	~Derived() override = default;
	double d_ = 3.14;
	std::string str_ = "Derived";
};

int main()
{
	constexpr int kSize = 10;

	Derived d[kSize];
	// Evil: d decays to &d[0] which is converted to a Base*
	Base* ptr1 = d;
	for (int i = 0; i < kSize; ++i)
	{
		// Error: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.
		std::cout << ptr1[i].b_ << std::endl;
		// Error: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.
		ptr1[i].b_ = 2;
	}

	Base* ptr2 = new Derived[kSize];
	// Error: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.
	delete[] ptr2;
	ptr2 = nullptr;

	return 0;
}
