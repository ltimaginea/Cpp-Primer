#include <iostream>
#include <memory>

void process(std::shared_ptr<int> ptr)
{
	std::cout << "inside the process function:" << ptr.use_count() << "\n";
}

int main()
{
	std::shared_ptr<int> p(new int(42));
	process(std::shared_ptr<int>(p));

	/**
	* codes below shows how the reference count change.
	*/
	std::cout << p.use_count() << "\n";
	auto q = p;
	std::cout << p.use_count() << "\n";
	std::cout << "the int p now points to is:" << *q << "\n";
	return 0;
}

/* Output:
inside the process function:2
1
2
the int p now points to is:42
*/

// tips: 
//   1. use_count 可能很慢，主要用于调试