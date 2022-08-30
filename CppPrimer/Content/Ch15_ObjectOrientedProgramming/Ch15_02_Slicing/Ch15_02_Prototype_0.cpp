#include <memory>
#include <typeinfo>
#include <cassert>

class Prototype
{
public:
	virtual std::unique_ptr<Prototype> Clone() const = 0;

	virtual ~Prototype() = default;

protected:
	Prototype() = default;

	// Make the polymorphic base class copy and move operations protected to prevent slicing, 
	// and so that only the derived class can invoke them in its own copy and move operations.
	Prototype(const Prototype&) = default;
	Prototype(Prototype&&) = default;
	Prototype& operator=(const Prototype&) = default;
	Prototype& operator=(Prototype&&) = default;
};

class Derived : public Prototype
{
public:
	std::unique_ptr<Prototype> Clone() const override
	{
		return std::make_unique<Derived>(*this);
	}
};

int main()
{
	std::unique_ptr<Prototype> p = std::make_unique<Derived>();
	std::unique_ptr<Prototype> q = p->Clone();

	assert((p != q) && (typeid(*p) == typeid(*q)) && (typeid(*q) == typeid(Derived)));

	return 0;
}
