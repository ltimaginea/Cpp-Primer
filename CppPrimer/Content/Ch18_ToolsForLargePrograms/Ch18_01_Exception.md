# Exception

如果试图在同一时刻令两个异常都处于活跃状态（在同一线程中，该用法被禁止），那么系统就不知道该处理哪个异常了：是你刚刚抛出的异常，还是它已经准备处理的异常？发生这种情况时，程序通常将调用标准库函数 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 终止当前的程序。比如，在栈展开的过程中，运行类类型的局部对象的析构函数。因为这些析构函数是自动执行的，所以它们不应该抛出异常。一旦在栈展开的过程中析构函数抛出了异常，并且析构函数自身没能捕获到该异常，则程序将调用 [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 终止程序。请注意，控制权（control）一旦进入到 catch 子句中，就表示准备处理异常了。重新抛出异常和在 catch 子句中抛出一个新异常都被认为是原来的异常被处理之后的新的抛出动作。

[std::terminate](https://en.cppreference.com/w/cpp/error/terminate) 被触发的常见几种情况有：（1）未捕获的异常。（2）当noexcept函数结束时仍然留有throw。（3）当栈展开期间的析构函数结束时仍然留有throw。



## References

- Bjarne Stroustroup's [*The C++ Programming Language*](http://www.amazon.com/The-Programming-Language-4th-Edition/dp/0321563840/) ,Chapter 13
- [Exceptions and Error Handling, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/exceptions)
- [Exception handling in MSVC | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/exception-handling-in-visual-cpp?view=msvc-170)
- [Modern C++ best practices for exceptions and error handling | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp?view=msvc-170)
- [std::terminate - cppreference.com](https://en.cppreference.com/w/cpp/error/terminate)

