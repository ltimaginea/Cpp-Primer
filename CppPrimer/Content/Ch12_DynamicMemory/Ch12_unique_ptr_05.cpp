#include <iostream>
#include <memory>

using namespace std;

int main()
{
	// up指向一个包含10个未初始化int的数组
	unique_ptr<int[]> up(new int[10]);
	for (size_t i = 0; i != 10; ++i)
	{
		// 用下标运算符来访问数组中的元素
		up[i] = i;
	}
	// 自动用 delete[] 销毁其指针
	up.reset();

	return 0;
}

// tips: 
//   1. 为了用一个unique_ptr管理动态数组，我们必须在对象类型后面跟一对空方括号[]，即 unique_ptr<T[]>