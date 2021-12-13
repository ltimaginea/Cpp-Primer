# HasPtrValueLike Case Not Exception Safe



```cpp
// 错误！无法处理自赋值的情况，同时还不是异常安全的！
HasPtr& HasPtr::operator=(const HasPtr& rhp)
{
	// free the old memory
	delete ps_;

	// 模拟new表达式抛出了异常
	std::string().at(1);
	// if rhp.ps_ is not nullptr, copy the underlying string
	std::string* newp = rhp.ps_ ? (new std::string(*rhp.ps_)) : nullptr;

	ps_ = newp;
	d_ = rhp.d_;

	return *this;
}
```

如上，不要把正确示例 [Ch13_02_HasPtrValueLike_better.cpp](./Ch13_02_HasPtrValueLike_better.cpp) 中的 拷贝右侧运算对象的资源 和 释放左侧运算对象的资源 的语句顺序调换，否则将会导致 **无法处理自赋值和不具备“异常安全性”**。

如果 `*this` 和 rhp 是同一个对象，`delete ps_` 会释放 `*this` 和 rhp 指向的 string 。接下来，当我们在 `new` 表达式中试图拷贝 `*rhp.ps_` 时，就会访问一个指向无效内存的指针，其行为和结果是未定义的。

这一版同时还不具备“异常安全性”，异常发生时左侧运算对象将不是置于一个有意义的状态。具体地说，如果 `new string(*rhp.ps_)` 抛出异常（不论是因为分配时内存不足或因为 std::string 的拷贝构造函数抛出异常）， `*this` 最终会持有一个指针指向一块被删除的 std::string 。这样的指针有害。我们无法安全地读取它们（解引用），并且**无法安全地删除它们——异常被捕获后， `*this` 离开其作用域时会发生 double free 这样的运行时错误（MSVC是其它的错误提示信息）**，详见下面的2个示例程序。

一些勉强的修补方法，但本质上还是要从 copy assignment operator 的实现做起，避免析构两次。

注意，下面的2个示例程序中，模拟的异常最好放置在new表达式之前。





```cpp
// case1: stack unwinding 时期发生 double free 
#include <iostream>
#include <string>
#include <exception>
#include <new>

class HasPtr
{
public:
	HasPtr(const std::string& s = std::string()) : ps_(new std::string(s)), d_(3.14) {  }
	HasPtr(const HasPtr& rhp) : ps_(rhp.ps_ ? (new std::string(*rhp.ps_)) : nullptr), d_(rhp.d_) {  }
	HasPtr& operator=(const HasPtr&);
	~HasPtr() { delete ps_; }
private:
	std::string* ps_;
	double d_;
};

// 错误！无法处理自赋值的情况，同时还不是异常安全的！
HasPtr& HasPtr::operator=(const HasPtr& rhp)
{
	// free the old memory
	delete ps_;

	// 模拟new表达式抛出了异常
	std::string().at(1);
	// if rhp.ps_ is not nullptr, copy the underlying string
	std::string* newp = rhp.ps_ ? (new std::string(*rhp.ps_)) : nullptr;

	ps_ = newp;
	d_ = rhp.d_;

	return *this;
}

void Test()
{
	HasPtr hp1("Jenny");
	HasPtr hp2("Hellooooooooooooooooooooooooooooooooo");
	hp1 = hp2;
}

int main()
{
	std::cout << "Test start!" << std::endl;
	try
	{
		Test();
	}
	catch (const std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exceptions" << std::endl;
	}
	std::cout << "Test end!" << std::endl;

	return 0;
}

/* GNU g++ Outputs:
Test start!
free(): double free detected in tcache 2
*/

```



```cpp
// case2: 局部对象离开其作用域时发生 double free 
#include <iostream>
#include <string>
#include <exception>
#include <new>

class HasPtr
{
public:
	HasPtr(const std::string& s = std::string()) : ps_(new std::string(s)), d_(3.14) {  }
	HasPtr(const HasPtr& rhp) : ps_(rhp.ps_ ? (new std::string(*rhp.ps_)) : nullptr), d_(rhp.d_) {  }
	HasPtr& operator=(const HasPtr&);
	~HasPtr() { delete ps_; }
private:
	std::string* ps_;
	double d_;
};

// 错误！无法处理自赋值的情况，同时还不是异常安全的！
HasPtr& HasPtr::operator=(const HasPtr& rhp)
{
	// free the old memory
	delete ps_;

	// 模拟new表达式抛出了异常
	std::string().at(1);
	// if rhp.ps_ is not nullptr, copy the underlying string
	std::string* newp = rhp.ps_ ? (new std::string(*rhp.ps_)) : nullptr;

	ps_ = newp;
	d_ = rhp.d_;

	return *this;
}

void Test()
{
	std::cout << "enter Test!" << std::endl;
	HasPtr hp1("Jenny");
	HasPtr hp2("Hellooooooooooooooooooooooooooooooooo");
	try
	{
		hp1 = hp2;
	}
	catch (const std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exceptions" << std::endl;
	}
	std::cout << "leave Test!" << std::endl;
}

int main()
{
	std::cout << "Test start!" << std::endl;
	Test();
	std::cout << "Test end!" << std::endl;

	return 0;
}

/* GNU g++ Outputs:
Test start!
enter Test!
basic_string::at: __n (which is 1) >= this->size() (which is 0)
leave Test!
free(): double free detected in tcache 2
*/

```

