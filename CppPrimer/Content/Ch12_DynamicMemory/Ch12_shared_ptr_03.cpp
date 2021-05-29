#include <iostream>
#include <vector>
#include <string>
#include <memory>


void process(std::shared_ptr<int> ptr)
{
	std::cout << "inside the process function:" << ptr.use_count() << "\n";
}

int main()
{
	auto p = new int();
	auto sp = std::make_shared<int>();

	// legal. Just copy sp which is a shared_ptr to process().
	// process(sp);

	// illegale.plain pointer can not convert to smart pointer implicitly.
	// process(new int());

	// illegale.plain pointer can not convert to smart pointer implicitly.
	// process(p);

	// Legal. But it's a bad practice to do so.Because using smart pointer together with raw pointer could potentially cause problems.
	// process(std::shared_ptr<int>(p));

	return 0;
}

// tips: 
//   1. 不要将同一块内存绑定到多个独立创建的shared_ptr上