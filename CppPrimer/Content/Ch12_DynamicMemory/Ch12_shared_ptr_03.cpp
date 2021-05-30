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
	auto p = new int(42);
	auto sp = std::make_shared<int>(422);

	// Legal. Just copy sp which is a shared_ptr to process().
	// process(sp);

	// Illegale. Plain pointer can not convert to smart pointer implicitly.
	// process(new int());

	// Illegale. Plain pointer can not convert to smart pointer implicitly.
	// process(p);

	// Legal. But memory will be freed, it's a bad practice to do so.Because using smart pointer together with raw pointer could potentially cause problems.
	// process(std::shared_ptr<int>(p));
	// Undefined: p is a dangling pointer.
	// int j = *p;

	// Illegale. std::shared_ptr<int>(sp.get()) construct a temporary shared_ptr and copy it to the parameter.
	// However it is not a copy of sp.As a result, at end of this main function sp will free the memory that has been freed inside process().
	// That's why "double freed or corruption" was generated.
	// process(std::shared_ptr<int>(sp.get()));

	// Illegale. Generate a runtime error : double free or corruption
	// delete sp.get();

	return 0;
}

// tips: 
//   1. 不要将同一块内存绑定到多个独立创建的shared_ptr上