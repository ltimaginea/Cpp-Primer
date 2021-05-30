#include <iostream>
#include <memory>

// C++11 手动实现一个基础功能版 make_unique ,这个基础函数不支持数组和自定义删除器
template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... args)
{
	return std::unique_ptr<T>{new T{ std::forward<Ts>(args)... }};
}

// tips: 
//   1. 从这个基础函数可以看出，make 函数把实参完美转发给构造函数并返回构造出的智能指针