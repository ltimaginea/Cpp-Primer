#include <iostream>
#include <string>
#include <vector>

#include <new>

using std::string;
using std::vector;

int main()
{
	int* p0 = nullptr;
	// 正确，释放一个空指针总是没有错误的
	delete p0;

	// 默认初始化，*p1的值未定义
	int* p1 = new int;
	delete p1;

	// 值初始化，*p2的值为0
	int* p2 = new int();
	delete p2;

	// p3的值为3.14
	double* p3 = new double(3.14);
	delete p3;

	// 默认初始化为空 string
	string* p4 = new string;
	delete p4;

	// 值初始化为空 string
	string* p5 = new string();
	delete p5;

	// 9999999999
	string* p6 = new string(10, '9');
	delete p6;

	// 使用花括号进行列表初始化
	vector<int>* p7 = new vector<int>{ 0,1,2,3,4,5,6,7,8,9 };
	delete p7;

	const int* p8 = new const int(1024);
	delete p8;

	// p9是一个double*
	auto p9 = new auto(6.28);
	// auto p99 = new auto(0, 1, 2);	// 错误，括号中只能有单个初始化器
	delete p9;

	// 10个未初始化的int
	int* p10 = new int[10];		// 分配一个10个int的数组，p10指向第一个int
	delete[] p10;

	// 值初始化，10个初始化为0的int
	typedef int arrT[10];
	int* p11 = new arrT();		// 分配一个10个int的数组，p11指向第一个int
	delete[] p11;

	// 值初始化，10个空string
	string* p12 = new string[10]();
	delete[] p12;

	// 10个string，前4个用给定的初始化器初始化，剩余的进行值初始化
	string* p13 = new string[10]{ "a","an","the",string(7,'x') };
	delete[] p13;

	// 错误！不能定义长度为0的数组
	// char arr[0];
	// 正确，但cp不能解引用
	char* cp = new char[0];
	delete[] cp;

	class Widget { double d[1024 * 1024 * 1024]; /* ... */ };

	try
	{
		// 如果分配失败，new抛出std::bad_alloc
		Widget* ptr = new Widget;

		try
		{
			//... perform some operations(may throw exceptions)
		}
		catch (...)			// for any exception
		{
			delete ptr;		// clean up
			throw;			// rethrow the exception
		}

		delete ptr;			// clean up on normal end
	}
	catch (const std::bad_alloc& err)
	{
		std::cout << err.what() << std::endl;
	}
	catch (const std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exceptions" << std::endl;
	}



	// 如果分配失败，new返回一个空指针
	Widget* ptr2 = new (std::nothrow) Widget;
	// 这个if...else...的判断可能有效
	if (ptr2 == nullptr)
	{
		std::cout << "new false" << std::endl;
	}
	else
	{
		// ...
	}
	delete ptr2;

	return 0;
}

// tips: 
//   1. 如果new分配失败，new抛出异常std::bad_alloc