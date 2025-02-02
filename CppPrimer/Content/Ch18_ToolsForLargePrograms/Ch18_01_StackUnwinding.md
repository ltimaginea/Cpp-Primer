# Stack Unwinding

当程序抛出一个异常时，程序暂停当前函数的执行过程并立即开始查找（look up）最邻近的与异常匹配的 catch 子句：

- 如果查找到一个匹配的 catch 子句，异常从它的抛出点开始“向上”传递到匹配的 catch 子句。异常传递过程中，当退出了某些作用域时，该作用域内异常发生前创建的局部对象会被销毁，按照与创建时相反的顺序依次销毁，对于类对象，销毁时会调用它的析构函数。上述过程称为**栈展开**（stack unwinding）。测试程序见 [Ch18_01_StackUnwinding.cpp](./Ch18_01_StackUnwinding.cpp) 。
- 如果没有查找到匹配的 catch 子句，即异常没有被捕获，那么 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 将会被调用以终止当前的程序。[N4971](https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2023/n4971.pdf): 14.4 Handling an exception: If no matching handler is found, the function std::terminate is invoked; **whether or not the stack is unwound before this invocation of std::terminate is implementation-defined. 当程序因未捕获的异常而终止时，是否发生栈展开是依赖于具体实现的（多数实现是不会进行栈展开的。）。** 至少如下两个环境：Windows 10 Visual Studio 2022 版本 17.11.5 MSVC C/C++ 编译器版本 19.41.34123 (compile with: `/EHsc` ) 和 Ubuntu 22.04 GCC 11.4.0 分别基于 C++20 标准测试，对于未捕获的异常，都不会发生栈展开。测试程序见 [Ch18_01_uncaught_exception.cpp](./Ch18_01_uncaught_exception.cpp) 。

**因此若需明确发生栈展开，则必须捕获异常**。同时还需要开启编译器的异常支持，例如 MSVC 的 `/EHsc` 选项、GCC 的 `-fexceptions` 选项（GCC enables it by default for C++ language.）。

为了能够快速处理异常，编译器应该会做一定的记录工作：在每一个 try 语句块的进入点记录对应的 catch 子句能够处理的异常类型。如果发生异常，程序在运行期便可以根据记录的数据来快速查找（look up）是否存在与异常匹配的 catch 子句，从而快速处理异常。不同的编译器的具体实现策略会有所不同。



[std::terminate](https://en.cppreference.com/w/cpp/error/terminate) : 终止当前的程序。默认情况下， [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 会调用 [std::abort](https://en.cppreference.com/w/cpp/utility/program/abort) 。当我们使用 [std::set_terminate](https://en.cppreference.com/w/cpp/error/set_terminate) 为 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 安装新的 [std::terminate_handler](https://en.cppreference.com/w/cpp/error/terminate_handler) 时，新安装的 [std::terminate_handler](https://en.cppreference.com/w/cpp/error/terminate_handler) 最终应该终止程序，如果没有， [std::abort](https://en.cppreference.com/w/cpp/utility/program/abort) 将会被自动调用以终止程序（经过使用 MSVC 和 g++ 测试，确实是这样。See: [Unhandled C++ exceptions | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/unhandled-cpp-exceptions?view=msvc-170) ）。

[std::abort](https://en.cppreference.com/w/cpp/utility/program/abort) : 导致程序异常终止。它不进行清理工作：不会调用自动对象，静态对象和线程局部对象的析构函数。

[std::exit](https://en.cppreference.com/w/cpp/utility/program/exit) : 导致程序正常终止。它会进行一些清理工作：会调用静态对象和线程局部对象的析构函数；但不会调用自动对象的析构函数。



[Exceptions and stack unwinding in C++ | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/exceptions-and-stack-unwinding-in-cpp?view=msvc-170) 的**栈展开**（stack unwinding）的描述如下：

In the C++ exception mechanism, control moves from the throw statement to the first catch statement that can handle the thrown type. When the catch statement is reached, all of the automatic variables that are in scope between the throw and catch statements are destroyed in a process that is known as *stack unwinding*. In stack unwinding, execution proceeds as follows:

1. Control reaches the **`try`** statement by normal sequential execution. The guarded section in the **`try`** block is executed.
2. If no exception is thrown during execution of the guarded section, the **`catch`** clauses that follow the **`try`** block are not executed. Execution continues at the statement after the last **`catch`** clause that follows the associated **`try`** block.
3. If an exception is thrown during execution of the guarded section or in any routine that the guarded section calls either directly or indirectly, an exception object is created from the object that is created by the **`throw`** operand. (This implies that a copy constructor may be involved.) At this point, the compiler looks for a **`catch`** clause in a higher execution context that can handle an exception of the type that is thrown, or for a **`catch`** handler that can handle any type of exception. The **`catch`** handlers are examined in order of their appearance after the **`try`** block. If no appropriate handler is found, the next dynamically enclosing **`try`** block is examined. This process continues until the outermost enclosing **`try`** block is examined.
4. If a matching handler is still not found, or if an exception occurs during the unwinding process but before the handler gets control, the predefined run-time function `terminate` is called. If an exception occurs after the exception is thrown but before the unwind begins, `terminate` is called. In these cases, it is implementation-defined whether any stack unwinding occurs at all: throwing an uncaught exception is permitted to terminate the program without invoking any destructors.
5. If a matching **`catch`** handler is found, and it catches by value, its formal parameter is initialized by copying the exception object. If it catches by reference, the parameter is initialized to refer to the exception object. After the formal parameter is initialized, the process of unwinding the stack begins. This involves the destruction of all automatic objects that were fully constructed—but not yet destructed—between the beginning of the **`try`** block that is associated with the **`catch`** handler and the throw site of the exception. Destruction occurs in reverse order of construction. The **`catch`** handler is executed and the program resumes execution after the last handler—that is, at the first statement or construct that is not a **`catch`** handler. Control can only enter a **`catch`** handler through a thrown exception, never through a **`goto`** statement or a **`case`** label in a **`switch`** statement.



## References

- Bjarne Stroustroup's [*The C++ Programming Language*](http://www.amazon.com/The-Programming-Language-4th-Edition/dp/0321563840/) ,Chapter 13
- [Exceptions and stack unwinding in C++ | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/exceptions-and-stack-unwinding-in-cpp?view=msvc-170)
- [Unhandled C++ exceptions | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/unhandled-cpp-exceptions?view=msvc-170)
- [/EH (Exception handling model) | Microsoft Learn](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)
- [try-block - cppreference.com](https://en.cppreference.com/w/cpp/language/try_catch)
- [throw expression - cppreference.com](https://en.cppreference.com/w/cpp/language/throw)
-  [std::terminate - cppreference.com](https://en.cppreference.com/w/cpp/error/terminate)
- [std::abort - cppreference.com](https://en.cppreference.com/w/cpp/utility/program/abort) 
- [std::exit - cppreference.com](https://en.cppreference.com/w/cpp/utility/program/exit)

