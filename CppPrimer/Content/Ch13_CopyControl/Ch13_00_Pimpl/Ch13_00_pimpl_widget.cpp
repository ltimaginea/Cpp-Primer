#include "Ch13_00_pimpl_widget.h"

#include <iostream>
#include <string>
#include <vector>
#include <utility>

//#include "gadget.h"

class Widget::Impl
{
public:
	Impl() : i_(0) {  }
	explicit Impl(int i) : i_(i) {  }

	void Draw(const Widget& w)
	{
		if (w.Shown()) // this call to public member function requires the back-reference
		{
			std::cout << "drawing a non-const widget " << i_ << std::endl;
		}
	}

	void Draw(const Widget& w) const
	{
		if (w.Shown()) // this call to public member function requires the back-reference
		{
			std::cout << "drawing a const widget " << i_ << std::endl;
		}
	}

	void PrintInfo() const { std::cout << i_ << std::endl; }

private:
	std::string name_;
	std::vector<double> data_;
	int i_;
	// ...
};

Widget::Widget() = default;
//Widget::Widget() : pimpl_(std::make_unique<Impl>(0))
//{
//
//}

Widget::Widget(int i) : pimpl_(std::make_unique<Impl>(i))
{

}

Widget::Widget(const Widget & rhs) : pimpl_(rhs.pimpl_ ? std::make_unique<Impl>(*rhs.pimpl_) : nullptr)
{

}

// 编译器合成的移动操作完全符合预期，所以我们使用 =default 来由编译器合成移动构造函数。
// 因为头文件 Ch13_00_pimpl_widget.h 中声明的移动构造函数未标记为 noexcept ，
// 所以编译器为使移动构造函数的声明和实现的“异常说明”一致，此例的合成移动构造函数将不会是 noexcept 。这算是Pimpl技术的一个缺点吧。
Widget::Widget(Widget&&) = default;

Widget& Widget::operator=(const Widget & rhs)
{
	pimpl_ = rhs.pimpl_ ? std::make_unique<Impl>(*rhs.pimpl_) : nullptr;
	return *this;
}

// 编译器合成的移动操作完全符合预期，所以我们使用 =default 来由编译器合成移动赋值运算符。
// 因为头文件 Ch13_00_pimpl_widget.h 中声明的移动赋值运算符未标记为 noexcept ，
// 所以编译器为使移动赋值运算符的声明和实现的“异常说明”一致，此例的合成移动赋值运算符将不会是 noexcept 。这算是Pimpl技术的一个缺点吧。
Widget& Widget::operator=(Widget&&) = default;

Widget::~Widget() = default;

void Widget::Draw()
{
	if (pimpl_) // null pointer check
	{
		pimpl_->Draw(*this);
	}
}

void Widget::Draw() const
{
	if (pimpl_) // null pointer check
	{
		// explicitly add low-level constness by const_cast
		const_cast<const Impl&>(*pimpl_).Draw(*this);

		// or:
		// const_cast<const Impl*>(pimpl_.get())->Draw(*this);
	}
}

void Widget::PrintInfo() const
{
	if (pimpl_) // null pointer check
	{
		// explicitly add low-level constness by const_cast
		const_cast<const Impl&>(*pimpl_).PrintInfo();

		// or:
		// const_cast<const Impl*>(pimpl_.get())->PrintInfo();
	}
}

// Tips:
//	1. 对于 std::unique_ptr 而言，删除器的类型是智能指针类型的一部分
//	2. 对于 std::shared_ptr 而言，删除器的类型并非智能指针类型的一部分
//	3. 编译器合成的移动操作完全符合预期，即针对 std::unique_ptr 执行移动操作，所以我们可以使用 =default 来让编译器生成合成版本的移动操作
