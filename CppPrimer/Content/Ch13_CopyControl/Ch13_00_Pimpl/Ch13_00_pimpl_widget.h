#pragma once
#include <memory>

class Widget
{
public:
	Widget(int i = 0);
	Widget(const Widget&);
	Widget(Widget&&);
	Widget& operator=(const Widget&);
	Widget& operator=(Widget&&);
	~Widget();
public:
	void PrintInfo();
	// ...
private:
	// things to be hidden go here
	class Impl;
	// opaque pointer to forward-declared class
	std::unique_ptr<Impl> pimpl_;
};

// Tips:
//	1. 对于 std::unique_ptr 而言，删除器的类型是智能指针类型的一部分
//	2. 对于 std::shared_ptr 而言，删除器的类型并非智能指针类型的一部分