# Return

## Return a non-reference type

返回一个值的方式和初始化一个变量或形参的方式完全一样：返回的值用于初始化调用点的一个临时量，该临时量就是函数调用的结果。

对于以下按值返回的函数 Foo ，我们不需要也不应该 `std::move()` 返回值（因为可能会对编译器的(N)RVO帮倒忙）。

```cpp
X Foo()
{
	X x;
	//...
	return x;
}
```

C++ Standard 指出，按值返回的函数（比如 Foo）保证有下列行为：

- 如果类型 X 有一个存在且可访问的（例如，不能是private的）copy或move构造函数，编译器可以选择略去其中的copy和move版本，通过直接在为函数返回值分配的内存上创建局部对象 x 来避免复制（或者移动）。这就是所谓的“返回值优化” *Return Value Optimization (RVO)* 或“命名返回值优化” *Named Return Value Optimization (NRVO)* ，这个特性甚至在C++11之前就获得了大多数编译器的支持。编译器对按值返回的函数实施(N)RVO需要满足两个前提条件：（1）局部对象与函数返回值的类型相同；（2）局部对象就是要返回的东西。适合的局部对象包括大多数局部变量（比如 Foo 里的 x ），还有作为`return`语句的一部分而创建的临时对象。函数形参不满足要求。
- 否则，如果X有一个move构造函数，X就被moved（搬移）。
- 否则，如果X有一个copy构造函数，X就被copied（复制）。
- 否则，报出一个编译期错误（compile-time error）。



## Return a reference type







> ## *References*
>
> ### 《深度探索C++对象模型》—— 2.3 返回值的初始化 & 在编译器层面做优化
>
> ### [Effective Modern Cpp Chinese / Item25.md](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/5.RRefMovSemPerfForw/item25.md)
>
> ### [Copy elision - cppreference.com](https://en.cppreference.com/w/cpp/language/copy_elision)
>
> Omits copy and move constructors, resulting in zero-copy pass-by-value semantics.
>
> ### [Rvalue Reference Declarator: && | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-160)
>
> Move semantics also helps when the compiler cannot use Return Value Optimization (RVO) or Named Return Value Optimization (NRVO). In these cases, the compiler calls the move constructor if the type defines it.
>
> 当编译器无法使用返回值优化 (RVO) 或命名返回值优化 (NRVO) 时，移动语义也很有用。 在这些情况下，如果类型定义了移动构造函数，则编译器将调用该函数。
>
> ### [/O1, /O2 (Minimize Size, Maximize Speed) VS2019| Microsoft Docs](https://docs.microsoft.com/en-us/cpp/build/reference/o1-o2-minimize-size-maximize-speed?view=msvc-160)
>
> Visual Studio 2019 的Debug模式的 **Optimization** 默认是 `已禁用 (/Od)` ，Release模式的 **Optimization** 默认是 `最大优化(优选速度) (/O2)` 。我们可以按照如下方法修改 **Optimization** 级别：
>
> To set this compiler option in the Visual Studio development environment:
>
> 1. Open the project's **Property Pages** dialog box. For details, see [Set C++ compiler and build properties in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/working-with-project-properties?view=msvc-160).
> 2. Select the **Configuration Properties** > **C/C++** > **Optimization** property page.
> 3. Modify the **Optimization** property.
>
> **/O1** and **/O2** also enable the Named Return Value optimization, which eliminates the copy constructor and destructor of a stack based return value.
>
> ### [/O1, /O2 (Minimize Size, Maximize Speed) VS2012| Microsoft Docs](https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2012/8f8h5cxt(v=vs.110))
>
> **/O1** and **/O2** also enable the Named Return Value optimization, which eliminates the copy constructor and destructor of a stack based return value.
>
> ### [Options Controlling C++ Dialect ( GCC Manual )](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/gcc/C_002b_002b-Dialect-Options.html#C_002b_002b-Dialect-Options)
>
> g++ 默认开启 (N)RVO ，我们可以通过编译选项 `-fno-elide-constructors` 来禁用优化。
>
> `-fno-elide-constructors`
>
> The C++ standard allows an implementation to omit creating a temporary that is only used to initialize another object of the same type. Specifying this option disables that optimization, and forces G++ to call the copy constructor in all cases. This option also causes G++ to call trivial member functions which otherwise would be expanded inline.
>
> In C++17, the compiler is required to omit these temporaries, but this option still affects trivial member functions.
>
> ### [C++ - Returning unique_ptr from functions - Stack Overflow](https://stackoverflow.com/questions/4316727/returning-unique-ptr-from-functions)
>
> ### [深入理解C++中的RVO-阿里云开发者社区 (aliyun.com)](https://developer.aliyun.com/article/705065?utm_content=g_1000226126)
>
> 

