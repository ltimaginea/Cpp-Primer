#include <iostream>

class B
{
public:
	virtual ~B() = default;

	virtual void f1(double) = 0;
	virtual void f11(double) const;
	virtual void f111(double) noexcept;
	virtual unsigned int f2(int) const;
	virtual int f3(int) &&;
	virtual void f4(double);
};

class D1 : public B
{
public:
	~D1() override = default;	// ok

	void f1(double) override;	// ok
	void f11(double) const override; // ok
	void f111(double) noexcept override; // ok
	unsigned int f2(int) const override;	// ok
	int f3(int) && override;	// ok
	void f4(double) final;	// ok

	int f5(double);
	int f6(double);
};

class D2 : public D1
{
public:
	~D2() override = default;	// ok

	void f1(int) override; // error: 'D2::f1' marked 'override', but does not override(the parameter types must be identical)
	void f11(double) override; // error: 'D2::f11' marked 'override', but does not override(the constness must be identical)
	void f111(double) override; // error: looser exception specification on overriding virtual function 'virtual void D2::f111(double)'
	int f2(int) const override; // error: 'D2::f2': overriding virtual function return type differs and is not covariant from 'D1::f2'
	int f3(int) & override; // error: 'D2::f3' marked 'override', but does not override(the reference qualifiers must be identical)
	void f4(double) override; // error: 'D1::f4': function declared as 'final' cannot be overridden by 'D2::f4'
	int f5(double) final; // error: 'D2::f5' marked 'final', but is not virtual
	int f6(double) override; // error: 'D2::f6' marked 'override', but does not override
	double f7() override; // error: 'D2::f7' marked 'override', but does not override
};

class NoDerived final { /* */ };
class Last final : public B { /* */ };
class Bad1 : public NoDerived { /* */ }; // error: cannot derive from 'final' base 'NoDerived' in derived type 'Bad1'
class Bad2 : public Last { /* */ }; // error: cannot derive from 'final' base 'Last' in derived type 'Bad2'

int main()
{

	return 0;
}
