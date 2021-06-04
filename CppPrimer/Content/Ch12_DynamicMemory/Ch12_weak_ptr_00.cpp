#include <iostream>
#include <memory>

using namespace std;

int main()
{
	auto sp = make_shared<int>(42);
	// wp弱共享sp,sp的引用计数未改变
	weak_ptr<int> wp = sp;
	if (auto p = wp.lock())
	{
		cout << *p << endl;
	}
	else
	{
		cout << "empty" << endl;
	}
	cout << wp.use_count() << endl;
	wp.reset();
	if (wp.expired())
	{
		cout << "expired" << endl;
	}
	else
	{
		cout << "not expired" << endl;
	}

	// 错误，不能解引用
	// cout << *wp << endl;
	// 错误，不能用作条件判断，检查其是否为空
	// if (wp)

	return 0;
}

// tips: 
//   1. lock是原子操作，但是use_count和expired是非线程安全