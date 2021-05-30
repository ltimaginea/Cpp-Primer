#include <iostream>
#include <memory>

struct Foo {
	Foo() { std::cout << "Foo...\n"; }
	~Foo() { std::cout << "~Foo...\n"; }
};

struct D {
	void operator() (Foo* p) {
		std::cout << "Calling delete for Foo object... \n";
		delete p;
	}
};

int main()
{
	std::cout << "Creating new Foo...\n";
	// up 占有 Foo 指针（删除器 D ）
	std::unique_ptr<Foo, D> up(new Foo(), D());

	std::cout << "Replace owned Foo with a new Foo...\n";
	// 调用旧者的删除器
	up.reset(new Foo());

	std::cout << "Release and delete the owned Foo...\n";
	up.reset(nullptr);
}

/* Output:
Creating new Foo...
Foo...
Replace owned Foo with a new Foo...
Foo...
Calling delete for Foo object...
~Foo...
Release and delete the owned Foo...
Calling delete for Foo object...
~Foo...
*/

// tips: 
//   1. reset: 如果unique_ptr不为空，则释放unique_ptr指向的对象；如果提供了内置指针参数，令unique_ptr重新指向给定的指针，否则将其置为空