# Static local variable and Global variable

## Static local variable

对于 static local variable ，在 control 首次经过其声明时才会被初始化（除非其初始化是零初始化或常量初始化，这可以在首次进入块前进行），在其后所有的调用中，声明均被跳过，在程序结束时其才会被销毁。

C++11起，static local variable 的初始化是线程安全的，C++11标准保证：如果多个线程试图同时初始化同一静态局部变量，则初始化严格只发生一次。见《C++ Concurrency in Action》Second Edition，Chapter 3.3.1，或者见 [Static local variables - cppreference.com](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) : 

> If multiple threads attempt to initialize the same static local variable concurrently, the initialization occurs exactly once.

static local variable 的一个应用便是C++11起的单例模式最佳实现：[Singleton](https://github.com/ltimaginea/Design-Patterns/blob/main/DesignPatterns/Singleton/Singleton.md)

## Global variable

C++11标准仅保证 static local variable 的初始化是线程安全的，对于 static non-local variable 是未提及的，即对于具有静态存储期的非局部变量，如类的 static data member 和 global variable ，它们的初始化会作为程序启动的一部分在 main 函数的执行之前进行：简单的类型会在编译期进行初始化，如 `constexpr` 类型，复杂的类型则需要在运行期初始化，如 复杂的类对象（初始化时需要调用构造函数）。在程序结束时它们才会被销毁。因为全局对象的初始化会作为程序启动的一部分在 main 函数的执行之前进行，所以其初始化不存在多个线程竞争初始化的问题，但是却存在异常安全和 initialization order 的问题。此外，初始化完成后，在多线程环境下，程序在使用 non-const 全局对象时还存在 data race 的问题。所以，我们应该尽量避免使用全局对象。

全局对象的异常安全的问题，具体来说是：任何措施都无法捕获全局对象的构造和析构过程中抛出的异常。

全局对象 initialization order 的问题，具体来说是：在单个翻译单元（translation unit）中，static non-local variable 的初始化始终严格以其定义出现于源代码中的顺序定序（销毁时以逆序销毁），但不同翻译单元中的 static non-local variable 的初始化之间是顺序不确定的（同样，销毁顺序也是不确定的，但会是以初始化时的逆序销毁）。如果某编译单元内的某个 static non-local variable 的初始化动作使用了另一编译单元内的某个 static non-local variable ，它所用到的这个对象可能尚未被初始化，因为C++对“定义于不同编译单元内的 static non-local variable ”的初始化次序并无明确定义（同样，由于不确定的销毁顺序，可能会导致某个全局对象销毁时，其析构函数访问一个已经销毁的全局对象），详见《Effective C++》Item4 和 见：

- [Initialization-Non-local_variables - cppreference.com](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables)

- [Static_and_Global_Variables (Google C++ Style Guide)](https://google.github.io/styleguide/cppguide.html#Static_and_Global_Variables)

- [I.2: Avoid non-const global variables (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-global)

- [I.3: Avoid singletons (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-singleton)

- [To-do: Unclassified proto-rules (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-unclassified) :

  > avoid static class members variables (race conditions, almost-global variables).

