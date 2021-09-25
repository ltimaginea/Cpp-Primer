# Move Semantics



一般来说，我们不需要为函数操作定义接受一个 `const T&&` 或是一个（普通的） `T&` 参数的版本。当我们希望从实参“窃取”数据时，通常传递一个右值引用。为了达到这一目的，实参不能是 const 的。类似的，从一个对象进行拷贝的操作不应该改变该对象。因此，通常不需要定义一个接受一个（普通的） T& 参数的版本。



## 移动右值，拷贝左值

如果一个类既有移动构造函数，也有拷贝构造函数，编译器使用普通的函数匹配规则来确定使用哪个构造函数。赋值操作的情况类似。

例如，在我们的 OverloadRule 类中，拷贝构造函数接受一个 `const OverloadRule&` 的引用，因此它可以用于任何可以转换为 OverloadRule 的类型，此版本从其参数拷贝数据。而移动构造函数接受一个 `OverloadRule&&` ，因此它只能用于实参是（非const）右值的情形，此版本会从其参数“窃取”数据。赋值操作的情况类似。

在实参是右值的情形下，copy操作和move操作都是可行的。但调用copy操作需要进行一次到const的转换，而move操作则是精确匹配（也是更好的匹配）的。因此，当我们传递一个可修改的右值时，编译器会选择运行move版本。

```cpp
#include <iostream>
#include <string>
#include <utility>

class OverloadRule
{
public:
	OverloadRule(std::string str = std::string()) :str_(str) {  }
	OverloadRule(const OverloadRule&) = default;
	OverloadRule(OverloadRule&&) = default;
	OverloadRule& operator=(const OverloadRule&) = default;
	OverloadRule& operator=(OverloadRule&&) = default;
	~OverloadRule() = default;
private:
	std::string str_;
};

int main()
{
	OverloadRule or1("courage"), or2("confidence");
	OverloadRule or3(or1);
	OverloadRule or4(std::move(or2));

	OverloadRule or5("spring"), or6("summer"), or7("autumn"), or8("winter");
	or5 = or6;
	or7 = std::move(or8);

	return 0;
}
```



## 如果没有移动操作，右值将被拷贝

如果一个类有一个拷贝构造函数但未定义移动构造函数，会发生什么呢？在此情况下，编译器不会合成移动构造函数，这意味着此类将有拷贝构造函数但不会有移动构造函数。如果一个类没有移动构造函数，函数匹配规则保证该类型的对象会被拷贝，即使我们试图通过调用 `std::move()` 来移动它们时也是如此。赋值操作的情况类似。copy操作是可行的，因为我们可以将一个 `T&&` 转换为一个 `const T&` 。

值得注意的是，用拷贝构造函数代替移动构造函数几乎肯定是安全的（赋值运算符的情况类似）。一般情况下，拷贝构造函数满足对应的移动构造函数的要求：它会拷贝给定对象，并将原对象置于有效状态。实际上，拷贝构造函数甚至都不会改变原对象的值。

```cpp
#include <iostream>
#include <string>
#include <utility>

class OverloadRule
{
public:
	OverloadRule(std::string str = std::string()) :str_(str) {  }
	OverloadRule(const OverloadRule&) = default;
	//OverloadRule(OverloadRule&&) = default;
	OverloadRule& operator=(const OverloadRule&) = default;
	//OverloadRule& operator=(OverloadRule&&) = default;
	~OverloadRule() = default;
private:
	std::string str_;
};

int main()
{
	OverloadRule or1("courage"), or2("confidence");
	OverloadRule or3(or1);
	OverloadRule or4(std::move(or2));

	OverloadRule or5("spring"), or6("summer"), or7("autumn"), or8("winter");
	or5 = or6;
	or7 = std::move(or8);

	return 0;
}
```

