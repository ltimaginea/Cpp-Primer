# default delete

## default

我们可以通过将特殊成员函数定义为 `=default` 来显式地要求编译器生成合成的版本，即我们只能对编译器可以合成的默认构造函数或拷贝控制成员使用 `=default` 。

当我们在类内用 `=default` 修饰成员的声明时，合成的函数将隐式地声明为内联的（就像任何其他类内声明的成员函数一样）。如果我们不希望合成的成员是内联函数，应该只对成员的类外定义使用 `=default` 。

当编译器合成拷贝控制成员时，同时也生成一个异常说明。如果对所有成员和基类的所有操作都承诺了不会抛出异常，则合成的成员是 `noexcept` 的。如果合成成员调用的任意一个函数可能抛出异常，则合成的成员是 `noexcept (false)` 。而且，如果我们定义了一个析构函数但是没有为它提供异常说明，则编译器将合成一个。合成的异常说明将与假设由编译器为类合成析构函数时所得的异常说明一致。（这段内容来自 *Cpp Primer* 18.1.4 。并且 [Move constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor) 的 Implicitly-declared move constructor 段落也有类似的描述：  The implicitly-declared (or defaulted on its first declaration) move constructor has an exception specification as described in [dynamic exception specification](https://en.cppreference.com/w/cpp/language/except_spec) (until C++17)[exception specification](https://en.cppreference.com/w/cpp/language/noexcept_spec) (since C++17). 所以，当我们使用 `=default` 来由编译器合成特殊成员函数时，不需要也不应该同时再手动添加“异常说明”，因为编译器总是做得更好，更有可能生成正确的“异常说明”，而如果我们手动添加了“异常说明”，那么也就阻止了编译器生成更有可能正确的“异常说明”了。测试示例程序见 [SynthesizedMoveConstructor_noexcept_false.cpp](./Ch13_01_SynthesizedMoveConstructor_noexcept_false.cpp) 和 [SynthesizedMoveConstructor_noexcept_true.cpp](./Ch13_01_SynthesizedMoveConstructor_noexcept_true.cpp) 。但对于 [Ch13_00_Pimpl](../Ch13_00_Pimpl) ，经过使用 Visual Studio 2019 和 g++ 9.3.0 测试，发现合成的移动操作并未标记为 `noexcept` ，推测原因为：因为头文件中声明的移动操作未标记为 `noexcept` ，所以当我们在实现文件中使用 `=default` 来由编译器合成移动操作时，编译器为使移动操作的声明和实现的“异常说明”一致，合成的移动操作将不会是 `noexcept` 的。）

```cpp
#include <iostream>
#include <string>

class T
{
public:
	T(std::string str) : str_(str) {  }
	T() = default;
	T(const T&);	// 仅声明
	T(T&&) = default;
	T& operator=(const T&) = default;
	T& operator=(T&&) = default;
	~T() = default;
private:
	std::string str_;
};

T::T(const T&) = default;	// 只对成员的类外定义使用 =default ，合成的成员将是非内联函数

int main()
{
	T t1("11");
	T t2("22");
	T t3("33");
	T t4("44");
	T t5(t1);
	T t6(std::move(t1));
	t1 = t2;
	t3 = std::move(t4);

	return 0;
}
```



## delete

**删除函数**（deleted function）是这样一种函数：我们虽然声明了它，但不能以任何方式使用它。在函数的参数列表后面加上 `=delete` 来指出我们希望将它定义为删除的。

对于函数来说，删除也是一种定义。

**通常，删除函数（deleted function）应该被声明为 `public` 而不是 `private`** 。这样做是有原因的。当客户代码尝试调用某个成员函数时，C++ 会先检查它的可访问性，后检查 *deleted* 状态。这么一来，当客户代码尝试调用某个 *private* 的删除函数时，有些编译器只会给出该函数是 *private* 的错误（而没有诸如该函数被 *deleted* 修饰的错误），即使函数的访问性不影响它是否能被使用。所以值得牢记，如果要将老代码的“ *private* 且未定义”函数替换为 *deleted* 函数时请一并修改它的访问性为 `public` ，这样可以让编译器产生更好的错误信息。

与 `=default` 不同，`=delete` 必须出现在函数第一次声明的时候（ [= delete - cppreference.com](https://en.cppreference.com/w/cpp/language/function#Deleted_functions) 也有相关的描述： The deleted definition of a function must be the first declaration in a translation unit: a previously-declared function cannot be redeclared as deleted）。

```cpp
struct sometype { sometype(); };
sometype::sometype() = delete; // error: must be deleted on the first declaration
```

与 `=default` 的另一个不同之处是，我们可以对任何函数指定 `=delete` （我们只能对编译器可以合成的默认构造函数或拷贝控制成员使用 `=default` ）。虽然删除函数的主要用途是禁止拷贝控制成员，但当我们希望引导函数匹配过程时，删除函数有时也是有用的。

我们可以通过将拷贝构造函数和拷贝赋值运算符定义为删除的函数来阻止拷贝，同时还可以通过将移动构造函数和移动赋值运算符定义为删除的函数来阻止移动。

```cpp
// Singleton 的典型实现
#include <iostream>
#include <cassert>

class Singleton
{
public:
	static Singleton& Instance()
	{
		static Singleton instance;
		return instance;
	}
public:
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;
private:
	Singleton() = default;
	~Singleton() = default;
};

int main()
{
	auto& a = Singleton::Instance();
	auto& b = Singleton::Instance();
	assert(&a == &b);

	return 0;
}
```



## private 拷贝控制

在 C++11 之前，类是通过将其拷贝构造函数和拷贝赋值运算符声明为 `private` 但并不定义它们来禁止拷贝的。

由于拷贝构造函数和拷贝赋值运算符是 `private` 的，用户代码将不能拷贝这个类型的对象。但是，友元和成员函数仍旧可以拷贝对象。为了阻止友元和成员函数进行拷贝，我们将这些拷贝控制成员声明为 `private` 的，但并不定义它们。 

声明但不定义一个成员函数是合法的。试图访问一个未定义的成员将导致一个链接时错误。通过声明为 `private` 但并不定义的拷贝构造函数和拷贝赋值运算符，我们可以预先阻止任何拷贝该类型对象的企图：试图拷贝对象的用户代码将在编译阶段被标记为错误；成员函数或友元函数中的拷贝操作将会导致链接时错误。

对于现代 C++ 程序，如果打算禁用某个函数，应该优先选用 `=delete` 删除函数，而非 `private` 未定义函数。

```cpp
// C++98 style的Singleton的典型实现
#include <iostream>
#include <cassert>

class Singleton
{
public:
	static Singleton* Instance()
	{
		if (instance_ == 0)
		{
			instance_ = new Singleton();
		}
		return instance_;
	}
public:
	~Singleton() {  }
private:
	// 将拷贝构造函数和拷贝赋值运算符声明为 private 但不定义它们，从而禁止拷贝操作
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
private:
	Singleton() {  }
private:
	static Singleton* instance_;
};

Singleton* Singleton::instance_ = 0;

int main()
{
	Singleton* a = Singleton::Instance();
	Singleton* b = Singleton::Instance();
	assert(a == b);

	// 完全不再使用后销毁Singleton实例的时机难以把握，当没有delete时会造成内存泄漏。
	// 当delete之后，对于那些已经获得Singleton实例的指针，它们将成为“空悬指针”，如程序中的 Singleton::instance_ , a 和 b 。
	delete Singleton::Instance();

	return 0;
}

// C++98 style的Singleton的缺点:
//	1. 完全不再使用后销毁Singleton实例的时机难以把握，当没有delete时会造成内存泄漏。
//	   当delete之后，对于那些已经获得Singleton实例的指针，它们将成为“空悬指针”，如示例中的 Singleton::instance_ , a 和 b 。
//	2. 存在线程安全的问题，因此还需要进一步优化（加锁）。
```



## References

* [C++ keywords: default - cppreference.com](https://en.cppreference.com/w/cpp/keyword/default)
* [= delete - cppreference.com](https://en.cppreference.com/w/cpp/language/function#Deleted_functions)
* [EffectiveModernCppChinese/item11.md at master · kelthuzadx/EffectiveModernCppChinese · GitHub](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/3.MovingToModernCpp/item11.md)
* [Move constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor)
* [is_nothrow_move_constructible - C++ Reference (cplusplus.com)](http://www.cplusplus.com/reference/type_traits/is_nothrow_move_constructible/)
* [std::is_move_constructible, std::is_trivially_move_constructible, std::is_nothrow_move_constructible - cppreference.com](https://en.cppreference.com/w/cpp/types/is_move_constructible)
* [c++ - Is the default Move constructor defined as noexcept? - Stack Overflow](https://stackoverflow.com/questions/18653726/is-the-default-move-constructor-defined-as-noexcept)
* [C.80: Use `=default` if you have to be explicit about using the default semantics (CppCoreGuidelines)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c80-use-default-if-you-have-to-be-explicit-about-using-the-default-semantics)
* [C.81: Use `=delete` when you want to disable default behavior (CppCoreGuidelines)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c81-use-delete-when-you-want-to-disable-default-behavior-without-wanting-an-alternative)

