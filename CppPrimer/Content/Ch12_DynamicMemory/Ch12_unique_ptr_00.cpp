#include <iostream>
#include <string>
#include <memory>

using namespace std;

// 不能拷贝unique_ptr的规则有一个例外：我们可以拷贝或赋值一个将要被销毁的unique_ptr
unique_ptr<double> clone(double p)
{
	if (p > 0)
	{
		// 从函数返回一个unique_ptr
		return unique_ptr<double>(make_unique<double>(p));
	}
	else
	{
		// 也可以返回一个局部对象的拷贝
		auto up(make_unique<double>(p));
		return up;
	}
}

int main()
{
	unique_ptr<int> p1(new int(42));
	// make_unique: C++14起
	unique_ptr<string> p2 = make_unique<string>(12, 'T');

	// 错误，unique_ptr 不支持拷贝
	// unique_ptr<string> p3(p2);
	unique_ptr<string> p4 = make_unique<string>("TianZhou2");
	// 错误，unique_ptr 不支持赋值
	// p4 = p2;

	// 通过调用release或reset将指针的所有权从一个（非const）unique_ptr转移给另一个unique
	unique_ptr<string> p5(p2.release());
	p5.reset(p4.release());
	// 错误，p5.release()不会释放内存，而且我们丢失了指针
	// p5.release();
	// 正确，但必须记得释放内存
	auto p = p5.release();
	delete p;

	return 0;
}

// tips: 
//   1. unique_ptr 不支持普通的拷贝或赋值操作