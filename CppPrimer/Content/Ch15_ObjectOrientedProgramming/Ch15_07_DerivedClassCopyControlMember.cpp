#include <iostream>
#include <string>
#include <utility>

class Base
{
	// ...
public:
	Base() = default;
	Base(const Base&) = default;
	Base(Base&&) = default;
	Base& operator=(const Base&) = default;
	Base& operator=(Base&&) = default;
	virtual ~Base() = default;
	// ...
};

class Derived : public Base
{
	// ...
public:
	Derived() = default;

	Derived(const Derived& rhs) : Base{ rhs }, derived_{ rhs.derived_ }
	{
		// Do the rest of the copy constructor here...
	}

	// If the move constructor doesn't throw an exception, it should be marked noexcept.
	Derived(Derived&& rhs) noexcept : Base{ std::move(rhs) }, derived_{ std::move(rhs.derived_) }
	{
		// Do the rest of the move constructor here...
	}

	Derived& operator=(const Derived& rhs)
	{
		// The copy assignment operator can gracefully handle self-assignment.
		if (this != &rhs)
		{
			Base::operator=(rhs);
			derived_ = rhs.derived_;
			// Do the rest of the copy assignment operator here...
		}
		return *this;
	}

	// If the move assignment operator doesn't throw an exception, it should be marked noexcept.
	Derived& operator=(Derived&& rhs) noexcept
	{
		// The move assignment operator can gracefully handle self-assignment.
		if (this != &rhs)
		{
			Base::operator=(std::move(rhs));
			derived_ = std::move(rhs.derived_);
			// Do the rest of the move assignment operator here...
		}
		return *this;
	}

	~Derived() override = default;
private:
	std::string derived_;
	// ...
};

int main()
{
	Derived d0, d1, d2;
	Derived d3{ d0 };
	Derived d4{ std::move(d0) };
	d1 = d2;
	d1 = std::move(d2);
	return 0;
}
