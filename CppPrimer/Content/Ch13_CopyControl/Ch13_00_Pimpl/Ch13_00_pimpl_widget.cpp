#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "Ch13_00_pimpl_widget.h"
//#include "gadget.h"

class Widget::Impl
{
public:
	Impl(int i = 0) :i_(i) {  }
public:
	int GetInfo() { return i_; }
private:
	std::string name_;
	std::vector<double> data_;
	int i_;
	// ...
};

Widget::Widget(int i) :pimpl_(std::make_unique<Impl>(i))
{

}

Widget::Widget(const Widget& rhs) : pimpl_(std::make_unique<Impl>(*rhs.pimpl_))
{

}

Widget::Widget(Widget&&) = default;

Widget& Widget::operator=(const Widget& rhs)
{
	pimpl_ = std::make_unique<Impl>(*rhs.pimpl_);
	return *this;
}

Widget& Widget::operator=(Widget&&) = default;

Widget::~Widget() = default;

void Widget::PrintInfo()
{
	std::cout << pimpl_->GetInfo() << std::endl;
}

// Tips:
//	1. 对于 std::unique_ptr 而言，删除器的类型是智能指针类型的一部分
//	2. 对于 std::shared_ptr 而言，删除器的类型并非智能指针类型的一部分
//	3. 编译器合成的移动操作完全符合预期，即针对 std::unique_ptr 执行移动操作，所以我们可以使用 =default 来让编译器生成合成版本的移动操作
