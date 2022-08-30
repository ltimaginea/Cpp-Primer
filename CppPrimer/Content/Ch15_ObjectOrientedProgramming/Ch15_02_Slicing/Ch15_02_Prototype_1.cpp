#include <memory>
#include <typeinfo>
#include <cassert>

class Prototype
{
public:
	// Note: a non-virtual function
	std::unique_ptr<Prototype> Clone() const
	{
		std::unique_ptr<Prototype> result = DoClone();
		assert(typeid(*result) == typeid(*this) && "Every derived class must correctly override DoClone.");
		return result;
	}

	virtual ~Prototype() = default;

protected:
	Prototype() = default;

	// Make the polymorphic base class copy and move operations protected to prevent slicing, 
	// and so that only the derived class can invoke them in its own copy and move operations.
	Prototype(const Prototype&) = default;
	Prototype(Prototype&&) = default;
	Prototype& operator=(const Prototype&) = default;
	Prototype& operator=(Prototype&&) = default;

private:
	virtual std::unique_ptr<Prototype> DoClone() const = 0;
};

class Derived1 : public Prototype
{
public:
	Derived1() = default;
	~Derived1() override = default;

protected:
	Derived1(const Derived1&) = default;
	Derived1(Derived1&&) = default;
	Derived1& operator=(const Derived1&) = default;
	Derived1& operator=(Derived1&&) = default;

private:
	std::unique_ptr<Prototype> DoClone() const override
	{
		// Note: create a temporary object
		return std::unique_ptr<Derived1>(new Derived1(*this));
	}
};

// Note: only a final derived class has public copy and move operations
class Derived2 final : public Derived1
{
public:
	Derived2() = default;
	Derived2(const Derived2&) = default;
	Derived2(Derived2&&) = default;
	Derived2& operator=(const Derived2&) = default;
	Derived2& operator=(Derived2&&) = default;
	~Derived2() override = default;

private:
	std::unique_ptr<Prototype> DoClone() const override
	{
		return std::make_unique<Derived2>(*this);
	}
};

int main()
{
	std::unique_ptr<Prototype> p = std::make_unique<Derived2>();
	std::unique_ptr<Prototype> q = p->Clone();

	assert((p != q) && (typeid(*p) == typeid(*q)) && (typeid(*q) == typeid(Derived2)));

	return 0;
}
