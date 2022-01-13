# override final

## override

基类中的虚函数在派生类中隐含地也是一个虚函数。当派生类覆盖（override）了某个虚函数时，该函数在基类中的形参必须与派生类中的**形参完全一致**。同样，派生类中虚函数的返回类型也必须与基类函数匹配。该规则存在一个例外，当类的虚函数返回类型是类本身的指针或引用时，上述规则无效。也就是说，如果 D 由 B 派生得到，则基类的虚函数可以返回 `B*` 而派生类的对应函数可以返回 `D*` ，只不过这样的返回类型要求从 D 到 B 的类型转换是可访问的。

**在C++11中，我们建议使用 `override` 标记派生类中的虚函数，如果其没有覆盖基类中已存在的虚函数，则编译器将报错**。

`override` 也可以应用在派生类的虚析构函数身上， 即 `~Derived() override = default;` 。

`override` 说明符出现在派生类内部的虚函数形参列表（包括任何 `const` 或引用修饰符）以及尾置返回类型之后，不能在类的外部定义虚函数时重复 `override` 。

## final

`final` 指定某个虚函数不能在派生类中被覆盖（override），或者某个类不能被派生。

注意，经过测试发现，当 `final` 用在虚函数身上时，`final` 仅确保指定的函数为 `virtual` 并且不可被派生类覆盖，`final` 不会像 `override` 一样进行检查指定的函数是否正确覆盖基类的同名同参数表的虚函数（不用太焦虑 `final` 的这个“问题”，因为只要我们坚持为虚函数中精确地指定 `virtual` , `override` , `final` 的其一，`final` 指定的函数就不会发生没有正确覆盖基类同名同参数表的虚函数的情况，因为 `final` 要求指定函数是虚函数，同时因为我们不会在 `final` 指定的函数上再重复手动指定为 `virtual` ，所以 `final` 指定的函数要想成为虚函数，它就必须正确覆盖了基类的虚函数才能够成为虚函数）。

`final` 说明符出现在派生类内部的虚函数形参列表（包括任何 `const` 或引用修饰符）以及尾置返回类型之后，不能在类的外部定义虚函数时重复 `final` 。 `final` 用在类身上指定其不能被派生时，同样地也不能重复：具体来说是，`final` 仅用在类的定义处，不能用在类的声明处。



## [C.128: Virtual functions should specify exactly one of virtual, override, or final (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override)

It’s simple and clear:

- `virtual` means exactly and only “this is a new virtual function.”
- `override` means exactly and only “this is a non-final overrider.”
- `final` means exactly and only “this is a final overrider.”

## Example

```cpp
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

```



## References

- [override specifier (since C++11) - cppreference.com](https://en.cppreference.com/w/cpp/language/override)
- [final specifier (since C++11) - cppreference.com](https://en.cppreference.com/w/cpp/language/final)
- [C.128: Virtual functions should specify exactly one of virtual, override, or final (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override)

