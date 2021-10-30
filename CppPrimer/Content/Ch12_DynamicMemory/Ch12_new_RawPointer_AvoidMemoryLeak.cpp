#include <iostream>
#include <new>

class Widget { double d[1024]; /* ... */ };

void Foo()
{
	Widget* ptr = new Widget;

	try
	{
		// perform some operations...(may throw exceptions)
		std::string().at(1);	// this generates an std::out_of_range
	}
	catch (...)			// for any exception
	{
		delete ptr;		// clean up
		throw;			// rethrow the exception
	}

	delete ptr;			// clean up on normal end
}

int main()
{
	try
	{
		Foo();
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

	return 0;
}
