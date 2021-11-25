#include <iostream>
#include <memory>

class Base
{
	friend class Pal;
public:
	virtual ~Base() = default;
protected:
	int GetState() { return state_; }
private:
	int state_ = 0;
};

//class Derived : private Base
//class Derived : protected Base
class Derived : public Base
{
	friend class Pri;
public:
	~Derived() override = default;
protected:
	int GetValue() { return value_; }
private:
	int value_ = 1;
};

class Pal_Base
{
public:
	virtual ~Pal_Base() = default;
	int f1(Base b) { b.GetState(); return b.state_; }		// Error
	int f2(Derived d) { d.GetState(); d.GetValue(); return d.value_; }	// Error
};

class Pal : public Pal_Base
{
public:
	~Pal() override = default;
	int f1(Base b) { return b.GetState(); }		// OK
	int f2(Derived d) { return d.GetValue(); }	// Error
	// f3确实是正确的。Pal是Base的友元，所以Pal能够访问Base对象的成员，这种可访问性包括了Base对象内嵌在其派生类对象中的情况(测试发现仅公有继承时该论述才正确)。
	int f3(Derived d) { return d.state_; }
};

class Pri
{
public:
	int f1(Derived d) { return d.value_; }
	int f2(Base b) { return b.GetState(); }		// Error
};

class D2 : public Pal
{
public:
	~D2() override = default;
	int f1(Base b) { b.state_; return b.GetState(); }		// Error
	int f2(Derived d) { d.value_; d.GetValue(); return d.GetState(); }	// Error
};

int main()
{
	return 0;
}
