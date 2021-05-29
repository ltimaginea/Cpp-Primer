#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>

using namespace std;

shared_ptr<int> clone(int p)
{
	// 正确，显式地用int*创建shared_ptr<int>，使用了直接初始化形式
	return shared_ptr<int>(new int(p));

	// 错误，int*不能隐式地转换为shared_ptr<int>
	// return new int(p);
}

int main()
{
	shared_ptr<int> p1;
	shared_ptr<list<int>> p2;
	// 在条件判断中使用智能指针，效果就是检测它是否为空，若它指向一个对象，则为true
	if (!p2)
		cout << "p2 is nullptr" << endl;

	// 正确，使用了直接初始化形式
	shared_ptr<int> p3(new int(1024));
	// 错误，必须使用直接初始化形式，使用 赋值号= 初始化一个变量，执行的是 拷贝初始化
	// shared_ptr<int> p4 = new int(1024);

	// 42
	shared_ptr<int> p5 = make_shared<int>(42);
	// 9999999999
	shared_ptr<string> p6 = make_shared<string>(10, '9');
	// 0
	shared_ptr<int> p7 = make_shared<int>();

	auto p = make_shared<double>(3.14);
	auto q(p);
	auto r = make_shared<double>(6.28);
	if (r.unique())
		cout << "r is only reference" << endl;
	// 给r赋值，令它指向另一个地址。递增q指向的对象的引用计数，递减r原来指向的对象的引用计数。r原来指向的对象已经没有引用者，会自动释放
	r = q;
	// 3
	cout << p.use_count() << endl;
	if (p.get() == r.get())
		cout << *r << endl;

	return 0;
}

// tips: 
//   1. 我们不能将一个内置指针隐式转换为一个智能指针，必须使用直接初始化形式来初始化一个智能指针