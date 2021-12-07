#include <iostream>
#include <memory>
#include <string>
#include <new>

class Except1
{
public:
	~Except1()
	{
		std::cout << "Except1's destructor is called!" << std::endl;
	}
private:
	std::string str_;
};

class Except2
{
public:
	~Except2()
	{
		std::cout << "Except2's destructor is called!" << std::endl;
	}
private:
	std::string str_;
};

class Except3
{
public:
	~Except3()
	{
		std::cout << "Except3's destructor is called!" << std::endl;
	}
private:
	std::string str_;
};

class Except4
{
public:
	~Except4()
	{
		std::cout << "Except4's destructor is called!" << std::endl;
	}
private:
	std::string str_;
};

void Test3()
{
	Except4 ex4;
	// throw an exception
	std::string().at(1);
	Except3 ex3;
}

void Test2()
{
	Except2 ex2;
	Test3();
}

void Test1()
{
	Except1 ex1;
	Test2();
}

int main()
{
	std::cout << "Test start" << std::endl;
	try
	{
		Test1();
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
	std::cout << "Test end" << std::endl;
	return 0;
}

/*
Outputs:
Test start
Except4's destructor is called!
Except2's destructor is called!
Except1's destructor is called!
basic_string::at: __n (which is 1) >= this->size() (which is 0)
Test end
*/

// tips: 
//   1. 异常必须要被捕获处理，未捕获异常将导致不会调用局部对象的析构函数（典型情况下如此，具体是否调用依赖于各个平台编译器的实现）
