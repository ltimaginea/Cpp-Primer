#include <iostream>
// uncomment to disable assert()
//#define NDEBUG
#include <cassert>

int main()
{
	// MSVC 和 MinGW-W64 都有预定义宏 _WIN32
#ifdef _WIN32
	std::cout << __cplusplus << std::endl;
#endif // _WIN32

	// MSVC 专用预定义宏
#ifdef _MSC_VER
	std::cout << _MSC_VER << std::endl;
#endif // _MSC_VER

	// g++ 专用预定义宏
#ifdef __GNUG__
	std::cout << __GNUG__ << __GNUC_MINOR__ << __GNUC_PATCHLEVEL__ << std::endl;
#endif // __GNUG__

	constexpr int i = 123;
	constexpr int j = 456;
	assert(i > j);
	static_assert(i > j, "i > j failed");

#ifndef NDEBUG
	std::cout << "i = " << i
		<< " j = " << j
		<< " : in function " << __func__
		<< std::endl;
#endif // !NDEBUG

	return 0;
}

// Tips:
//	1. 默认状态下没有定义NDEBUG，此时assert将执行运行时检查
