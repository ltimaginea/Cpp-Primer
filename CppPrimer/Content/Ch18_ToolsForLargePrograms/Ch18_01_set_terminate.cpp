#include <iostream>
#include <string>
#include <memory>
#include <new>

class Demo
{
public:
	~Demo() { std::cout << "Demo destructor is called" << std::endl; }
};

void Test()
{
	static std::shared_ptr<Demo> demo_ptr = std::make_shared<Demo>();

	// throw an exception
	std::string().at(1);
}

int main()
{
	std::set_terminate([]() { std::cout << "uncaught exception" << std::endl; std::exit(3); });
	std::cout << "Test start" << std::endl;
	try
	{
		Test();
	}
	// won't catch the exception instance of 'std::out_of_range' by 'std::string().at(1);' throw
	catch (const std::bad_alloc& err)
	{
		std::cout << err.what() << std::endl;
	}
	/*catch (const std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exceptions" << std::endl;
	}*/
	std::cout << "Test end" << std::endl;
	return 0;
}

/* GNU g++ Outputs:
Test start
uncaught exception
Demo destructor is called
*/
