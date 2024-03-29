# Copy Control

## 合成的拷贝控制成员（可能是删除的）

编译器合成的特殊成员函数都具有 public 访问层级且是 inline 的，而且它们都是非虚函数，除非讨论的是一个析构函数，位于一个派生类中，并且基类的析构函数是个虚函数。在那种情况下，编译器为派生类合成的析构函数也是个虚函数。

编译器合成的操作依次处理每个非 static 数据成员，根据成员类型确定如何拷贝、移动、赋值或销毁它。

当编译器合成拷贝控制成员时，同时也生成一个异常说明。如果对所有成员和基类的所有操作都承诺了不会抛出异常，则合成的成员是 `noexcept` 的。如果合成成员调用的任意一个函数可能抛出异常，则合成的成员是 `noexcept (false)` 。而且，如果我们定义了一个析构函数但是没有为它提供异常说明，则编译器将合成一个。合成的异常说明将与假设由编译器为类合成析构函数时所得的异常说明一致。（这段内容来自 *Cpp Primer* 18.1.4 。并且 [Move constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor) 的 Implicitly-declared move constructor 段落也有类似的描述：  The implicitly-declared (or defaulted on its first declaration) move constructor has an exception specification as described in [dynamic exception specification](https://en.cppreference.com/w/cpp/language/except_spec) (until C++17)[exception specification](https://en.cppreference.com/w/cpp/language/noexcept_spec) (since C++17).  所以，当我们使用 `=default` 来由编译器合成特殊成员函数时，不需要也不应该同时再手动添加“异常说明”，因为编译器总是做得更好，更有可能生成正确的“异常说明”，而如果我们手动添加了“异常说明”，那么也就阻止了编译器生成更有可能正确的“异常说明”了。测试示例程序见 [SynthesizedMoveConstructor_noexcept_false.cpp](./Ch13_01_default_delete/Ch13_01_SynthesizedMoveConstructor_noexcept_false.cpp) 和 [SynthesizedMoveConstructor_noexcept_true.cpp](./Ch13_01_default_delete/Ch13_01_SynthesizedMoveConstructor_noexcept_true.cpp) 。但对于 [Ch13_00_Pimpl](./Ch13_00_Pimpl) ，经过使用 Visual Studio 2019 和 g++ 9.3.0 测试，发现合成的移动操作并未标记为 `noexcept` ，推测原因为：因为头文件中声明的移动操作未标记为 `noexcept` ，所以当我们在实现文件中使用 `=default` 来由编译器合成移动操作时，编译器为使移动操作的声明和实现的“异常说明”一致，合成的移动操作将不会是 `noexcept` 的。）

只有特殊成员函数被需要（被调用）时，它们才会被编译器合成。

对于特殊成员函数来说，删除也是一种定义。

### 合成的拷贝操作

**如果一个类未定义任何构造函数，编译器会为其定义一个合成默认构造函数。类似的，如果类未定义 拷贝构造函数、拷贝赋值运算符和析构函数，编译器总会为我们合成这些操作，它们要么被定义为逐成员拷贝，要么被定义为逐成员赋值，要么被定义为逐成员析构，要么被定义为删除的函数**。

对某些类来说，编译器将这些合成的特殊成员函数定义为删除的函数：

- 如果类的某个成员的析构函数是删除的或不可访问的（例如，是private的），则类的合成析构函数被定义为删除的。
- 如果类的某个成员的拷贝构造函数是删除的或不可访问的，则类的合成拷贝构造函数被定义为删除的。如果类的某个成员的析构函数是删除的或不可访问的，则类合成的拷贝构造函数也被定义为删除的。
- 如果类的某个成员的拷贝赋值运算符是删除的或不可访问的，或是类有一个const的或引用成员，则类的合成拷贝赋值运算符被定义为删除的。
- 如果类的某个成员的析构函数是删除的或不可访问的，或是类有一个引用成员，它没有类内初始化器，或是类有一个const成员，它没有类内初始化器且其类型未显式定义默认构造函数，则该类的默认构造函数被定义为删除的。 

本质上，这些规则的含义是：**如果一个类有数据成员不能默认构造、拷贝、赋值或销毁，则对应的成员函数将被定义为删除的**。 

**一个成员有删除的或不可访问的析构函数会导致合成的默认、拷贝构造函数和移动构造函数被定义为删除的**，这看起来可能有些奇怪。其原因是，如果没有这条规则，我们可能会创建出无法销毁的对象。 

对于具有引用成员或无法默认构造的const成员的类，编译器不会为其合成默认构造函数，这应该不奇怪。同样不出人意料的规则是：如果一个类有const成员，则它不能使用合成的拷贝赋值运算符。毕竟，此运算符试图赋值所有成员，而将一个新值赋予一个const对象是不可能的。 

虽然我们可以将一个新值赋予一个引用成员，但这样做改变的是引用指向的对象的值，而不是引用本身。如果为这样的类合成拷贝赋值运算符，则赋值后，左侧运算对象仍然指向与赋值前一样的对象，而不会与右侧运算对象指向相同的对象。由于这种行为看起来并不是我们所期望的，因此对于有引用成员的类，合成拷贝赋值运算符被定义为删除的。

### 合成的移动操作

编译器合成的移动构造函数执行的是逐成员移动构造，同时还会移动构造它的基类部分（如果有的话）；编译器合成的移动赋值运算符执行的是逐成员移动赋值，同时还会移动赋值它的基类部分（如果有的话）。“按成员移动”实际上更像是按成员的移动请求，因为对于那些不可移动的类型将通过其复制操作实现“移动”。

**只有当一个类没有定义任何自己版本的拷贝控制成员（删除也是一种定义），且类的每个非static数据成员都可以移动时，编译器才会为它合成移动构造函数或移动赋值运算符**。编译器可以移动内置类型的成员。如果一个成员是类类型，且该类有对应的移动操作，编译器也能移动这个成员。

与拷贝操作不同，移动操作永远不会隐式定义为删除的函数。但是，**如果我们显式地要求编译器生成 `=default` 的移动操作，且编译器不能移动所有成员，则编译器会将移动操作定义为删除的函数**。除了一个重要例外（指下面的第一个原则），什么时候将合成的移动操作定义为删除的函数遵循与定义删除的合成拷贝操作类似的原则：

- 与拷贝构造函数不同，移动构造函数被定义为删除的函数的条件是：有类成员定义了自己的拷贝构造函数且未定义移动构造函数，或者是有类成员未定义自己的拷贝构造函数且编译器不能为其合成移动构造函数。移动赋值运算符的情况类似。
- 如果有类成员的移动构造函数或移动赋值运算符被定义为删除的或是不可访问的，则类的移动构造函数或移动赋值运算符被定义为删除的。
- 类似拷贝构造函数，如果类的析构函数被定义为删除的或不可访问的，则类的移动构造函数被定义为删除的。
- 类似拷贝赋值运算符，如果有类成员是 const 的或是引用，则类的移动赋值运算符被定义为删除的。

移动操作和**合成的**拷贝控制成员间还有最后一个相互作用关系：一个类是否定义了自己的移动操作对拷贝操作如何合成有影响。**如果类定义了一个移动构造函数 和/或 一个移动赋值运算符（删除也是一种定义），则该类的合成拷贝构造函数和拷贝赋值运算符 都 会被定义为删除的**。（注：这里的意思是指 对**合成的**拷贝操作有影响，即如果没有显式地定义拷贝构造函数和拷贝赋值运算符，那么结果就是：“该类的**合成**拷贝构造函数和拷贝赋值运算符 **都** 会被定义为删除的”。而如果我们有显式地定义拷贝操作，即 自定义的拷贝操作或者使用 `=default` 来显式地要求编译器生成合成的拷贝操作，那么结果将是以我们显式定义的为准。其中，对于使用 `=default` 来显式地要求编译器生成合成的拷贝操作，那么结果将是 该类的合成拷贝构造函数和拷贝赋值运算符的定义情况将取决于该类具体的情况（参见 [Ch13_00_CopyControl.md 合成的拷贝操作](./Ch13_00_CopyControl.md#合成的拷贝操作) ），即合成的拷贝操作将可能是“有效的”，举例： [Ch13_01_default_delete/Ch13_01_default.cpp](./Ch13_01_default_delete/Ch13_01_default.cpp) 。）

举例：

```cpp
#include <iostream>
#include <string>
#include <utility>

class Y
{
public:
	Y() = default;
	Y(Y&&) = delete;	// 删除的移动构造函数
	Y& operator=(Y&&) = default;
private:
	std::string str_;
};

class HasY
{
public:
	HasY() = default;
	HasY(HasY&&) = default;	// 最终编译器会将HasY的移动构造函数定义为删除的
	HasY& operator=(HasY&&) = default;	// OK
private:
	Y mem_;	// Y的移动构造函数是删除的
};

int main()
{
	HasY hy1;
	HasY hy2(std::move(hy1));	// 错误！HasY移动构造函数是删除的

	HasY hy3, hy4;
	hy3 = std::move(hy4);	// OK

	HasY hy5, hy6;
	// error! 'HasY::HasY(const HasY&)' is implicitly declared as deleted because 'HasY' declares a move constructor or move assignment operator
	HasY hy7(hy5);
	// error! 'HasY& HasY::operator=(const HasY&)' is implicitly declared as deleted because 'HasY' declares a move constructor or move assignment operator
	hy5 = hy6;

	return 0;
}
```

分析：（1）类HasY拥有一个不能移动构造的类型为Y的成员，而我们又显式地要求编译器生成`=default`的移动构造函数，但编译器无法为其生成。因此，HasY会有一个删除的移动构造函数。如果HasY忽略了移动构造函数的声明，则编译器根本不能为它合成一个。如果移动操作可能被定义为删除的函数，编译器就不会合成它们。 （2）类HasY的类型为Y的成员定义了移动赋值运算符，因此我们可以显式地要求编译器生成`=default`的移动赋值运算符，其是正确的，因此类HasY可以正确进行移动赋值。（3）同时我们还可以看到，因为类HasY定义了一个移动构造函数 和/或 一个移动赋值运算符，所以类HasY的合成拷贝构造函数和拷贝赋值运算符 都 被定义为删除的了，因此示例的最后部分，类HasY的拷贝构造和拷贝赋值时是会报错的。



> ## *References*
>
> [The rule of three/five/zero - cppreference.com](https://en.cppreference.com/w/cpp/language/rule_of_three)
>
> [三/五/零之法则 - cppreference.com](https://zh.cppreference.com/w/cpp/language/rule_of_three)
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

