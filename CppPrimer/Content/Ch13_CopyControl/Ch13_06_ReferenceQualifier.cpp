#include <iostream>
#include <vector>
#include <algorithm>

class Foo
{
public:
	Foo(std::vector<int> data = std::vector<int>()) :data_(data) {  }
	// 只能向可修改的左值赋值
	Foo& operator=(const Foo& f) &
	{
		data_ = f.data_;
		return *this;
	}

	void SomeMem() const & {  }		// 正确！const 限定符在前
	//void SomeMem() & const {  }	// 错误！const 限定符必须在前

	Foo Sorted() &&;
	Foo Sorted() const &;	// 正确！这两个相同参数列表的Sorted都有引用限定符

	using Comp = bool(const int&, const int&);
	Foo Sorted(Comp*);
	Foo Sorted(Comp*) const;		// 正确！这两个相同参数列表的Sorted都没有引用限定符
	//Foo Sorted(Comp*) const &;		// 错误！相同参数列表的Sorted应该同时具有或缺少引用限定符

private:
	std::vector<int> data_;
};

// 本对象为右值，因此可以原址排序
Foo Foo::Sorted() &&
{
	std::sort(data_.begin(), data_.end());
	return *this;
}

// 本对象是 const 或是一个左值，哪种情况我们都不能对其进行原址排序
Foo Foo::Sorted() const &
{
	// 拷贝一个副本
	Foo ret(*this);
	// 排序副本
	std::sort(ret.data_.begin(), ret.data_.end());
	// 返回副本
	return ret;
}

Foo& RetFoo()
{
	static Foo f(std::vector<int>{1, 2, 3});
	return f;
}

Foo RetVal()
{
	return Foo(std::vector<int>{1, 2, 3});
}

int main()
{
	Foo i, j;
	// 正确！i 是左值
	i = j;

	// 正确！RetFoo() 返回一个左值
	RetFoo() = j;
	// 错误！RetVal() 返回一个右值
	//RetVal() = j;

	i = RetVal();

	// RetVal() 是一个右值，调用 Foo Sorted() &&
	RetVal().Sorted();
	// RetFoo() 是一个左值，调用 Foo Sorted() const &
	RetFoo().Sorted();

	return 0;
}

// Tips:
//	1. 当我们对一个右值执行sorted时，它可以安全地直接对data成员进行排序。对象是一个右值，意味着没有其他用户，因此我们可以改变对象。
//	2. 当对一个const右值或一个左值执行sorted时，我们不能改变对象，因此就需要在排序前拷贝data。
//	3. 编译器会根据调用sorted的对象的左值/右值属性来确定使用哪个sorted版本。
