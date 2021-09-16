# Return

## Return a non-reference type

返回一个值的方式和初始化一个变量或形参的方式完全一样：返回的值用于初始化调用点的一个临时量，该临时量就是函数调用的结果。

我们不需要也不应该 `std::move()` 返回值（因为可能会对编译器的RVO帮倒忙）。C++ Standard 指出，对于以下代码：

```cpp
X Foo()
{
	X x;
	//...
	return x;
}
```

保证有下列行为：

- 如果X有一个存在且可访问的（例如，不能是private的）copy或move构造函数，编译器可以选择略去其中的copy版本。这也就是所谓的 Return Value Optimization (RVO) or Named Return Value Optimization (NRVO) ，这个特性甚至在C++11之前就获得了大多数编译器的支持。
- 否则，如果X有一个move构造函数，X就被moved（搬移）。
- 否则，如果X有一个copy构造函数，X就被copied（复制）。
- 否则，报出一个编译期错误（compile-time error）。

## Return a reference type







> ## *References*
>
> ### [Rvalue Reference Declarator: && | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-160)
>
> Move semantics also helps when the compiler cannot use Return Value Optimization (RVO) or Named Return Value Optimization (NRVO). In these cases, the compiler calls the move constructor if the type defines it.
>
> 当编译器无法使用返回值优化 (RVO) 或命名返回值优化 (NRVO) 时，移动语义也很有用。 在这些情况下，如果类型定义了移动构造函数，则编译器将调用该函数。
>
> ### [Options Controlling C++ Dialect ( GCC Manual )](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/gcc/C_002b_002b-Dialect-Options.html#C_002b_002b-Dialect-Options)
>
> `-fno-elide-constructors`
>
> The C++ standard allows an implementation to omit creating a temporary that is only used to initialize another object of the same type. Specifying this option disables that optimization, and forces G++ to call the copy constructor in all cases. This option also causes G++ to call trivial member functions which otherwise would be expanded inline.
>
> In C++17, the compiler is required to omit these temporaries, but this option still affects trivial member functions.
>
> ### [EffectiveModernCppChinese/item25.md](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/5.RRefMovSemPerfForw/item25.md)
>
> ### [C++ - Returning unique_ptr from functions - Stack Overflow](https://stackoverflow.com/questions/4316727/returning-unique-ptr-from-functions)
>
> ### 《深度探索C++对象模型》
>
> 

