# static members

示例程序1：(1) [Ch07_06_StaticMembers.h](Ch07_06_StaticMembers.h) (2) [Ch07_06_StaticMembers.cpp](Ch07_06_StaticMembers.cpp) (3) [Ch07_06_StaticMembers_main.cpp](Ch07_06_StaticMembers_main.cpp) 

示例程序2： [Xinbiaozhun_cpp_Chengxusheji/StaticMember.cpp](https://github.com/ltimaginea/Xinbiaozhun_cpp_Chengxusheji/blob/master/XinBiaoZhun_Cpp/Chapter3_Constructor_Destructor/StaticMember.cpp) 

正文示例代码为程序1。

## 声明静态成员

我们通过在成员的声明之前加上关键字 `static` 使得其与类关联在一起。和其他成员一样，静态成员可以是 `public` 的或 `private` 的。静态数据成员的类型可以是常量、引用、指针、类类型等。

类的静态成员存在于任何对象之外，对象中不包含任何与静态数据成员有关的数据。静态数据成员只有一份，被所有同类对象共享，本质上是全局变量。因此， `sizeof()` 运算符计算类对象所占的存储空间时，不会将静态数据成员计算在内。

类似的，静态成员函数也不与任何对象绑定在一起，它们不包含 `this` 指针，本质上是全局函数。作为结果，静态成员函数不能声明成 `const` 的，而且我们也不能在 `static` 函数体内使用 `this` 指针。这一限制既适用于 `this` 的显式使用，也对调用非静态成员的隐式使用有效，所以静态成员函数内部不能访问非静态成员变量，也不能调用非静态成员函数。

```cpp
// file.h
class X
{
public:
	static void Foo();	// declaration (uses 'static')
private:
	static int n_;		// declaration (uses 'static')
};

// file.cpp
void X::Foo()	// definition (does not use 'static')
{
	n_ = 2;
}
int X::n_ = 1;	// definition (does not use 'static')
```

## 使用类的静态成员

我们可以使用作用域运算符直接访问静态成员：

```cpp
double r = Account::GetRate();
```

虽然静态成员不属于类的某个对象，但是我们仍然可以使用类的对象、引用或者指针来访问静态成员：

```cpp
Account ac;
double r = ac.GetRate();

auto ac_ptr = std::make_unique<Account>();
r = ac_ptr->GetRate();
```

成员函数内部不用通过作用域运算符就能直接使用静态成员，如示例程序1 Account 类的 Calculate 函数定义中，直接使用了静态成员 interest_rate_ ：

```cpp
void Calculate() { amount_ += amount_ * interest_rate_; }
```

## 定义静态成员

和其他的成员函数一样，我们既可以在类的内部也可以在类的外部定义静态成员函数。**定义在类内部的静态成员函数是自动 `inline` 的，不会导致重复定义的问题。当在类的外部定义静态成员时，不能重复 `static` 关键字，该关键字只出现在类内部的声明语句**。如示例程序1 Account 类的 SetRate 函数的类外部定义：

```cpp
void Account::SetRate(double new_rate)
{
	interest_rate_ = new_rate;
}
```

和类的所有成员一样，当我们指向类外部的静态成员时，必须指明成员所属的类名。 `static` 关键字则只出现在类内部的声明语句中。 

因为静态数据成员不属于类的任何一个对象，所以它们并不是在创建类的对象时被定义的。这意味着它们不是由类的构造函数初始化的。而且**一般来说，我们不能在类的内部初始化静态数据成员。相反的，必须在类的外部定义和初始化每个静态数据成员。和其他对象一样，一个静态数据成员只能定义一次。要想确保对象只定义一次，最好的办法是把静态数据成员的定义与其他非内联成员函数的定义放在同一个 .cpp 实现文件中**。

类似于全局变量，静态数据成员定义在任何函数之外。因此一旦它被定义，就将一直存在于程序的整个生命周期中。 

我们定义静态数据成员的方式和在类的外部定义成员函数差不多。我们需要指定对象的类型名，然后是类名、作用域运算符以及成员自己的名字：

```cpp
// 定义并初始化静态数据成员
double Account::interest_rate_ = InitRate();
```

这条语句定义了名为 interest_rate_ 的对象，该对象是类 Account 的静态成员，其类型是 double 。从类名开始，这条定义语句的剩余部分就都位于类的作用域之内了。因此，我们可以直接使用 InitRate 函数。注意，虽然 InitRate 是私有的，我们也能用它初始化 interest_rate_ 。和其他成员的定义一样， interest_rate_ 的定义也可以访问类的私有成员。 

## 常量静态数据成员的类内初始化

通常情况下，类的静态成员不应该在类的内部初始化。然而，我们可以为静态成员提供 `const` 整数类型的类内初始值，不过要求静态成员必须是字面值常量类型的 `constexpr` 。初始值必须是常量表达式，因为这些成员本身就是常量表达式，所以它们能用在所有适合于常量表达式的地方。例如示例程序1类 Account 中，我们用一个初始化了的常量静态数据成员来指定数组成员的维度。其实对于“指定数组成员的维度”这样的功能需求，更好的选择是在该类体内定义一个枚举类型，由枚举类型的成员来指定数组的维度，从而避免使用类的静态数据成员。

**常量静态数据成员，最佳实践就是在类内部进行初始化，然后在类外部进行定义。即使一个常量静态数据成员在类内部被初始化了，通常情况下也应该在类的外部定义一下该成员，这样一来该常量静态数据成员的应用场景将会是通用的。如果在类的内部提供了一个初始值，则类外部的成员的定义不能再指定一个初始值了。**

详见《C++ Primer》7.6节 P270至P271 和 [const static members - cppreference.com](https://en.cppreference.com/w/cpp/language/static#Constant_static_members) 。

## static members 特殊场景

如上所述，静态成员独立于任何对象。因此，在某些非静态数据成员可能非法的场合，静态成员却可以正常地使用。比如，**静态数据成员可以是不完全类型（incomplete type）。特别的，静态数据成员的类型可以就是它所属的类类型。而非静态数据成员则受到限制，只能声明成它所属类的指针或引用。**

```cpp
class Bar
{
public:
	// ...
private:
	static Bar mem1_;	// Ok, 静态成员可以是不完全类型
	Bar* mem2_;		// Ok, 指针成员可以是不完全类型
	Bar mem3_;		// Error, 数据成员必须是完全类型
};
```

静态成员和普通成员的另外一个区别是我们可以使用静态成员作为默认实参。非静态数据成员不能作为默认实参，因为它的值本身属于对象的一部分，这么做的结果是无法真正提供一个对象以便从中获取成员的值，最终将引发错误。

```cpp
class Screen
{
public:
	// bkground_ 表示在一个类中稍后定义的静态成员
	Screen& clear(char ch = bkground_) { /*...*/ return *this; }
private:
	static const char bkground_ = 'A';
};
const char Screen::bkground_;
```

## avoid class static data members

我们应该避免使用类的 static data member ，因为其本质上是全局对象，全局对象存在一些缺点：（1）多个编译单元内的全局对象的构造和析构顺序是未定义的。（2）任何措施都无法捕获全局对象的构造和析构中抛出的异常。（3）多线程环境下，使用 non-const 的全局对象时，存在 data race 。

详见： [Ch02_02_StaticLocalVariable_GlobalVariable.md](../Ch02_VariablesAndBasicTypes/Ch02_02_StaticLocalVariable_GlobalVariable.md)

## References

- [static members - cppreference.com](https://en.cppreference.com/w/cpp/language/static)
- [static local variables - cppreference.com](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)
- [Initialization-Non-local_variables - cppreference.com](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables)
- [Static_and_Global_Variables (Google C++ Style Guide)](https://google.github.io/styleguide/cppguide.html#Static_and_Global_Variables)
- [I.2: Avoid non-const global variables (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-global)
- [I.3: Avoid singletons (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-singleton)
- [SF.2: A .h file must not contain object definitions or non-inline function definitions (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rs-inline)
- [SF.8: Use #include guards for all .h files (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rs-guards)

