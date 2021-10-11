# default delete

## default

我们可以通过将特殊成员函数定义为 `=default` 来显式地要求编译器生成合成的版本，即我们只能对编译器可以合成的默认构造函数或拷贝控制成员使用 `=default` 。

当我们在类内用 `=default` 修饰成员的声明时，合成的函数将隐式地声明为内联的（就像任何其他类内声明的成员函数一样）。如果我们不希望合成的成员是内联函数，应该只对成员的类外定义使用 `=default` 。

当编译器合成拷贝控制成员时，同时也生成一个异常说明。如果对所有成员和基类的所有操作都承诺了不会抛出异常，则合成的成员是 `noexcept` 的。如果合成成员调用的任意一个函数可能抛出异常，则合成的成员是 `noexcept (false)` 。而且，如果我们定义了一个析构函数但是没有为它提供异常说明，则编译器将合成一个。合成的异常说明将与假设由编译器为类合成析构函数时所得的异常说明一致。（这段内容来自 *Cpp Primer* 18.1.4 。并且 [Move constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor) 的 Implicitly-declared move constructor 段落也有类似的描述：  The implicitly-declared (or defaulted on its first declaration) move constructor has an exception specification as described in [dynamic exception specification](https://en.cppreference.com/w/cpp/language/except_spec) (until C++17)[exception specification](https://en.cppreference.com/w/cpp/language/noexcept_spec) (since C++17).  测试示例程序见 [SynthesizedMoveConstructor_noexcept_false.cpp](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_01_default_delete/Ch13_01_SynthesizedMoveConstructor_noexcept_false.cpp) 和 [SynthesizedMoveConstructor_noexcept_true.cpp](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_01_default_delete/Ch13_01_SynthesizedMoveConstructor_noexcept_true.cpp) 。 [Pimpl](https://github.com/ltimaginea/Cpp-Primer/tree/main/CppPrimer/Content/Ch13_CopyControl/Ch13_00_Pimpl) 算是一个特例，因为头文件中声明的移动操作未标记为 `noexcept` ，所以当我们在实现文件中使用 `=default` 来由编译器合成移动操作时，编译器为使移动操作的声明和实现的“异常说明”一致，合成的移动操作将不会是 `noexcept` 的。）

```cpp
#include <iostream>
#include <string>

class T
{
public:
	T(std::string str = std::string()) :str_(str) {  }
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



与 `=default` 不同，`=delete` 必须出现在函数第一次声明的时候（ [= delete - cppreference.com](https://en.cppreference.com/w/cpp/language/function#Deleted_functions) 也有相关的描述： The deleted definition of a function must be the first declaration in a translation unit: a previously-declared function cannot be redeclared as deleted）。

```cpp
struct sometype { sometype(); };
sometype::sometype() = delete; // error: must be deleted on the first declaration
```





> ## References
>
> [C++ keywords: default - cppreference.com](https://en.cppreference.com/w/cpp/keyword/default)
>
> [= delete - cppreference.com](https://en.cppreference.com/w/cpp/language/function#Deleted_functions)
>
> [Move constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor)
>
> [is_nothrow_move_constructible - C++ Reference (cplusplus.com)](http://www.cplusplus.com/reference/type_traits/is_nothrow_move_constructible/)
>
> [std::is_move_constructible, std::is_trivially_move_constructible, std::is_nothrow_move_constructible - cppreference.com](https://en.cppreference.com/w/cpp/types/is_move_constructible)
>
> [c++ - Is the default Move constructor defined as noexcept? - Stack Overflow](https://stackoverflow.com/questions/18653726/is-the-default-move-constructor-defined-as-noexcept)
>
> 

