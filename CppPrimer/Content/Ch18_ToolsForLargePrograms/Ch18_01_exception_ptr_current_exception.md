# exception_ptr current_exception

详见《The C++ Standard Library》(A Tutorial and Reference,second edition) 的4.3.3节。

自 C++11 起，C++ 标准库提供一个能力：将异常存储于类型为 `exception_ptr` 的对象中，稍后才在其他情境（context）中处理它们。

`current_exception()` 会返回一个 `exception_ptr` 对象，指向当前正被处理的异常。该异常会保持有效，直到没有任何 `exception_ptr` 指向它。 `rethrow_exception()` 会重新抛出那个被存储的异常，

这个特性在我们打算于线程之间传递异常时特别有用。

```cpp
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

void handle_eptr(std::exception_ptr eptr) // passing by value is ok
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		std::cout << "Caught exception \"" << e.what() << "\"\n";
	}
}

int main()
{
	std::exception_ptr eptr;
	try {
		std::string().at(1); // this generates an std::out_of_range
	}
	catch (...) {
		eptr = std::current_exception(); // capture
	}
	handle_eptr(eptr);
} // destructor for std::out_of_range called here, when the eptr is destructed
```



## References

- [std::current_exception - cppreference.com](https://en.cppreference.com/w/cpp/error/current_exception)
- [std::exception_ptr - cppreference.com](https://en.cppreference.com/w/cpp/error/exception_ptr)
- [try-block - cppreference.com](https://en.cppreference.com/w/cpp/language/try_catch)
- [exception - C++ Reference (cplusplus.com)](http://www.cplusplus.com/reference/exception/exception/)

