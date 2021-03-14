#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
* 类成员变量不能直接捕获，如果想通过lambda方式访问类中的成员，可以通过在捕获列表中显式添加this指针。
* 当出现任一默认捕获符(&或=)时，也都能隐式捕获当前对象(*this)。当它被隐式捕获时，始终被以引用捕获，即使默认捕获符是 = 也是如此。当默认捕获符为 = 时，*this 的隐式捕获被弃用(C++20 起)。
* 参考: https://en.cppreference.com/w/cpp/language/lambda
*/
struct X
{
	X() :some_data(100) {}
	int some_data;
	void foo(std::vector<int>& vec)
	{
		std::for_each(vec.begin(), vec.end(),
			[this](int& i) { i += some_data; });
	}
};

int main()
{
	X test;
	vector<int> vec = { 1,2,3,4,5,6,7,8,9 };
	test.foo(vec);
	for_each(vec.begin(), vec.end(), [](int i) { cout << i << " "; });
	return 0;
}