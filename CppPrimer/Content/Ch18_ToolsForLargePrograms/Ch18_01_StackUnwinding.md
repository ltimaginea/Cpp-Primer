# Stack Unwinding

当程序抛出一个异常时，程序暂停当前函数的执行过程并立即开始查找（look up）最邻近的与异常匹配的 catch 子句。

- 如果查找到一个匹配的 catch 子句，异常从它的抛出点开始“向上”传递到匹配的 catch 子句。异常传递过程中，当退出了某些作用域时，该作用域内异常发生前创建的局部对象会被销毁，按照与创建时相反的顺序依次销毁，对于类对象，销毁时会调用它的析构函数。上述过程称为**栈展开**（stack unwinding）。
- 如果没有查找到匹配的 catch 子句，即异常没有被捕获，程序将调用标准库函数 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) ，它将终止当前的程序。默认情况下，  [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 会调用 [std::abort](https://en.cppreference.com/w/cpp/utility/program/abort) 。出于底层操作系统方面的原因，当调用 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 时局部变量的析构函数是否会被调用是由具体C++实现所决定的。**所以当程序因未捕获的异常而终止时，是否调用异常发生前创建的局部对象的析构函数是依赖于具体实现的**（一方面，经过测试，对于 GNU g++ 9.3.0 ，执行和 gdb 调试时都不会调用析构函数；对于 Visual Studio 2022 MSVC ，“`Ctrl+F5`执行(不调试)”时不会调用析构函数，但在“`F5`调试”时，当报错“未经处理的异常”时，选择“`F5`继续”，结果会调用析构函数；另一方面，如果我们使用 [std::set_terminate](https://en.cppreference.com/w/cpp/error/set_terminate) 为 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 安装新的 [std::terminate_handler](https://en.cppreference.com/w/cpp/error/terminate_handler) ，那么就有可能调用析构函数了，比如以 [std::exit](https://en.cppreference.com/w/cpp/utility/program/exit) 替换默认的 [std::abort](https://en.cppreference.com/w/cpp/utility/program/abort) 作为新的 [std::terminate_handler](https://en.cppreference.com/w/cpp/error/terminate_handler) ，同时如果局部变量是 `static` 的，那么程序因未捕获的异常而终止时就会调用局部 `static` 变量的析构函数了）。



[std::abort](https://en.cppreference.com/w/cpp/utility/program/abort) : 不会进行清理工作，即不会调用自动对象，静态对象和线程局部对象的析构函数。

[std::exit](https://en.cppreference.com/w/cpp/utility/program/exit) : 会进行清理工作，即会调用静态对象和线程局部对象的析构函数；但不进行栈展开（stack unwinding），即不会调用自动对象的析构函数。



[Exceptions and stack unwinding in C++ | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/exceptions-and-stack-unwinding-in-cpp?view=msvc-170) 的**栈展开**（stack unwinding）的描述如下：

In the C++ exception mechanism, control moves from the throw statement to the first catch statement that can handle the thrown type. When the catch statement is reached, all of the automatic variables that are in scope between the throw and catch statements are destroyed in a process that is known as *stack unwinding*. In stack unwinding, execution proceeds as follows:

1. Control reaches the **`try`** statement by normal sequential execution. The guarded section in the **`try`** block is executed.
2. If no exception is thrown during execution of the guarded section, the **`catch`** clauses that follow the **`try`** block are not executed. Execution continues at the statement after the last **`catch`** clause that follows the associated **`try`** block.
3. If an exception is thrown during execution of the guarded section or in any routine that the guarded section calls either directly or indirectly, an exception object is created from the object that is created by the **`throw`** operand. (This implies that a copy constructor may be involved.) At this point, the compiler looks for a **`catch`** clause in a higher execution context that can handle an exception of the type that is thrown, or for a **`catch`** handler that can handle any type of exception. The **`catch`** handlers are examined in order of their appearance after the **`try`** block. If no appropriate handler is found, the next dynamically enclosing **`try`** block is examined. This process continues until the outermost enclosing **`try`** block is examined.
4. If a matching handler is still not found, or if an exception occurs during the unwinding process but before the handler gets control, the predefined run-time function `terminate` is called. If an exception occurs after the exception is thrown but before the unwind begins, `terminate` is called. In these cases, it is implementation-defined whether any stack unwinding occurs at all: throwing an uncaught exception is permitted to terminate the program without invoking any destructors.
5. If a matching **`catch`** handler is found, and it catches by value, its formal parameter is initialized by copying the exception object. If it catches by reference, the parameter is initialized to refer to the exception object. After the formal parameter is initialized, the process of unwinding the stack begins. This involves the destruction of all automatic objects that were fully constructed—but not yet destructed—between the beginning of the **`try`** block that is associated with the **`catch`** handler and the throw site of the exception. Destruction occurs in reverse order of construction. The **`catch`** handler is executed and the program resumes execution after the last handler—that is, at the first statement or construct that is not a **`catch`** handler. Control can only enter a **`catch`** handler through a thrown exception, never through a **`goto`** statement or a **`case`** label in a **`switch`** statement.



## References

- [Exceptions and stack unwinding in C++ | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/exceptions-and-stack-unwinding-in-cpp?view=msvc-170)
- [try-block - cppreference.com](https://en.cppreference.com/w/cpp/language/try_catch)
- [throw expression - cppreference.com](https://en.cppreference.com/w/cpp/language/throw)
-  [std::terminate - cppreference.com](https://en.cppreference.com/w/cpp/error/terminate)
- [std::abort - cppreference.com](https://en.cppreference.com/w/cpp/utility/program/abort) 
- [std::exit - cppreference.com](https://en.cppreference.com/w/cpp/utility/program/exit)

