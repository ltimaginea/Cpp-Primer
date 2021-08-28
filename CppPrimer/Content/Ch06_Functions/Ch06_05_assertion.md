# assertion

## assert

`assert` 是一种预处理宏（preprocessor marco）。所谓预处理宏其实是一个预处理变量，它的行为有点类似于内联函数。assert宏使用一个表达式作为它的条件：

```cpp
assert(expr);
```

**首先对expr求值，如果表达式为假（即0），assert输出信息并终止程序的执行。如果表达式为真（即非0），assert什么也不做。** 

assert宏定义在cassert头文件中。如我们所知，预处理名字由预处理器而非编译器管理，因此我们可以直接使用预处理名字而无须提供using声明。也就是说，我们应该使用assert而不是std::assert，也不需要为assert提供using声明。 

和预处理变量一样，宏名字在程序内必须唯一。含有cassert头文件的程序不能再定义名为assert的变量、函数或者其他实体。



## NDEBUG

**assert的行为依赖于一个名为 `NDEBUG` 的预处理变量的状态。如果定义了NDEBUG，则assert什么也不做。默认状态下没有定义NDEBUG，此时assert将执行运行时检查。** 

我们可以使用一个#define语句定义NDEBUG，从而关闭调试状态。当在文件中进行定义时，我们应该将  `#define NDEBUG` 写在cassert头文件的位置之前。

**更推荐使用命令行选项定义预处理器变量NDEBUG ，这样便可以无需更改代码就能开启或关闭assert机制。**

```bash
g++ -D NDEBUG main.cpp
```

这条命令的作用等价于在 main.cpp 文件的一开始写 `#define NDEBUG`  。

除了用于assert外，也可以使用NDEBUG编写自己的条件调试代码。如果NDEBUG未定义，将执行#ifndef和#endif之间的代码；如果定义了NDEBUG，这些代码将被忽略掉。



## static_assert

C++11中新增了 `static_assert` ，可以进行编译时断言检查。

```cpp
static_assert( constant-expression, string-literal );
static_assert( constant-expression ); // C++17
```

常量表达式需要可以转换为布尔值。如果计算的表达式为零 (false)，则显示字符串参数并且编译失败并显示错误。如果表达式非零 (true)，则static_assert声明无效。

static_assert 声明可以出现在命名空间和块作用域中（作为块声明），也可以在类体中（作为成员声明）。

static_assert 最重要的用途是为泛型编程中作为形参的类型设置断言，因为模板参数可以包含在常量表达式参数中。



> ## *References*
>
> [assert - cppreference.com](https://en.cppreference.com/w/cpp/error/assert)
>
> [static_assert declaration - cppreference.com](https://en.cppreference.com/w/cpp/language/static_assert)
>
> [Assertion and User-Supplied Messages (C++) | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/assertion-and-user-supplied-messages-cpp?view=msvc-160)
>
> [static_assert | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/static-assert?view=msvc-160)
>
> [Predefined macros | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-160)
>
> [Standard Predefined Macros (The C Preprocessor) (gnu.org)](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/cpp/Standard-Predefined-Macros.html#Standard-Predefined-Macros)
>
> [Common Predefined Macros (GNU C extensions) (gnu.org)](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/cpp/Common-Predefined-Macros.html#Common-Predefined-Macros)
>
> [Predefined Macros (The C Preprocessor) (gnu.org)](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/cpp/Predefined-Macros.html#Predefined-Macros)
>
> [Preprocessor Options (Using the GNU Compiler Collection (GCC))](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/gcc/Preprocessor-Options.html#Preprocessor-Options)
>
> [Code Gen Options (Using the GNU Compiler Collection (GCC))](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/gcc/Code-Gen-Options.html#Code-Gen-Options)
>
> [GCC 11.2 Manual (gnu.org))](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/gcc/)
>
> [GCC 11.2 CPP Manual (gnu.org)](https://gcc.gnu.org/onlinedocs/gcc-11.2.0/cpp/)
>
> [GCC online documentation - GNU Project](https://gcc.gnu.org/onlinedocs/)
>
> [GCC, the GNU Compiler Collection - GNU Project](https://www.gnu.org/software/gcc/)
>
> 

