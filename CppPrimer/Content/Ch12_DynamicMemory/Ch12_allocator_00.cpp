#include <iostream>
#include <memory>
#include <string>

int main()
{
	using std::string;
	using std::allocator;
	using std::cout;
	using std::endl;

	size_t n = 10;
	allocator<string> alloc;
	// allocates uninitialized storage. Throws std::bad_alloc if allocation fails.
	// Return value: Pointer to the first element of an array of n objects of type T whose elements have not been constructed yet.
	auto const p = alloc.allocate(n);
	auto q = p;

	// constructs an object in allocated storage.
	alloc.construct(q++);	// empty string
	// cccccccccc
	alloc.construct(q++, 10, 'c');
	// hi
	alloc.construct(q++, "hi");

	while (q != p)
	{
		cout << *(--q) << endl;
		// destructs an object in allocated storage
		alloc.destroy(q);
	}

	// deallocates storage
	alloc.deallocate(p, n);

	return 0;
}

// tips: 
//   1. 为了使用allocate返回的内存，我们必须用construct构造对象。使用未构造的内存，其行为是未定义的。
//   2. 我们只能对真正构造了的元素进行destroy操作。