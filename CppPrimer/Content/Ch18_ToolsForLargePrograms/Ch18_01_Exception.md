# Exception

异常处理的正确观念是：

1. **仅捕获自己所能处理的异常**。异常处理的目的在于从异常中恢复运行：运行时捕获异常，运行时解决问题，程序继续运行。
2. 而对于一个无法处理的异常，就不应该捕获或者捕获后部分处理（如free相关内存或日志打印异常what信息）再抛出，然后由更上层的专门的错误处理层进行捕获处理，或直至抛至线程入口函数处触发 std::terminate() ，让程序终止运行！程序终止后，根据异常信息，离线排查此异常为何会发生，离线解决问题，程序重新启动运行。

**如果异常在线程中未被捕获，系统将调用std::terminate()**。因此，如果我们不希望整个程序因线程中的一个异常而终止执行，就必须在线程入口函数（主线程则是main函数）中进行捕获全部异常，并且以某种方式把它们报告给程序中对线程执行结果感兴趣的部分。“捕获全部异常”catch(...)有助于实现该目标。我们可以用标准库函数 std::current_exception 和 std::promise::set_exception 把某一线程的异常传递给另一线程的处理程序。

如果试图在同一时刻令两个异常都处于活跃状态（在同一线程中，该用法被禁止），那么系统就不知道该处理哪个异常了：是你刚刚抛出的异常，还是它已经准备处理的异常？发生这种情况时，程序通常将调用标准库函数 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 终止当前的程序。比如，在栈展开期间，执行类类型的局部对象的析构函数时：因为这些析构函数是自动执行的，所以它们不应该抛出异常，一旦在栈展开的过程中析构函数抛出了异常，并且析构函数自身没能捕获到该异常，则程序将调用 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 终止程序。请注意，控制权（control）一旦进入到 catch 子句中，就表示准备处理异常了。因此在 catch 子句中重新抛出异常或者抛出一个新异常都被认为是原来的异常被处理之后的新的抛出动作。

[std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 被触发的常见几种情况有：（1）未捕获的异常。（2）当noexcept函数结束时仍然留有throw。（3）当栈展开期间的析构函数结束时仍然留有throw。

## References

- Bjarne Stroustroup's [*The C++ Programming Language*](http://www.amazon.com/The-Programming-Language-4th-Edition/dp/0321563840/) ,Chapter 13
- [Exceptions and Error Handling, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/exceptions)
- [Exception handling in MSVC | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/exception-handling-in-visual-cpp?view=msvc-170)
- [Modern C++ best practices for exceptions and error handling | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp?view=msvc-170)
- [Transporting exceptions between threads | Microsoft Learn](https://learn.microsoft.com/en-us/cpp/cpp/transporting-exceptions-between-threads?view=msvc-170)
- [std::terminate - cppreference.com](https://en.cppreference.com/w/cpp/error/terminate)
- [std::promise::set_exception - cppreference.com](https://en.cppreference.com/w/cpp/thread/promise/set_exception)

