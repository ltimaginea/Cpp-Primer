#include <iostream>
#include <memory>

using namespace std;

int main()
{
	// 为了使用shared_ptr管理一个动态数组，必须提供自己定义的删除器
	shared_ptr<int> sp(new int[10], [](int* ptr) { delete[] ptr; });
	// C++11中，shared_ptr未定义下标运算符
	for (size_t i = 0; i != 10; ++i)
	{
		sp.get()[i] = i;
		//*(sp.get() + i) = i;	// 同上等价
	}
	for (size_t i = 0; i != 10; ++i)
	{
		cout << *(sp.get() + i) << " ";
	}
	// 使用我们自定义的删除器lambda释放数组，它使用 delete[]
	sp.reset();

	return 0;
}

// tips: 
//   1. 如果希望使用shared_ptr管理一个动态数组，必须提供自己定义的删除器，自定义删除器内部需要使用delete[]释放数组。