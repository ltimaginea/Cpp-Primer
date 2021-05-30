#include <iostream>
#include <vector>
#include <string>
#include <memory>

int main()
{
	int ix = 1024, * pi = &ix, * pi2 = new int(2048);
	typedef std::unique_ptr<int> IntP;

	// error: invalid conversion from 'int' to 'std::unique_ptr<int>::pointer { aka int* }' [-fpermissive]
	// IntP p0(ix);

	// The code below can compile, but will cause error at run time.
	// The reason is that when the unique_ptr p1 is out of scope, delete will be called to free th object.But the object is not allocate using new.
	// Thus, an error would be thrown by operating system.
	// IntP p1(pi);

	// This code can compile, but cause a dangling pointer at run time.
	// The reason is that the unique_ptr will free the object the raw pointer is pointing to.
	// { IntP p2(pi2); }

	// When the unique_ptr goes out of scope, it will call delete to free an obeject not allocated using new.
	// IntP p3(&ix);

	// Recommended.
	// IntP p4(new int(2048));

	// error: double free or corruption at run time two unique_ptr are pointing to the same object.
	// Thus, when both are out of scope, Operating system will throw double free or corruption.
	IntP p5(new int(555));
	// error: double free or corruption
	// IntP p6(p5.get());
	// Recommended.
	IntP p7(p5.release());

	return 0;
}

// tips: 
//   1. p.reset(p.release()) 的代码不涉及自重置，只有类似 p.reset(p.get()) 的代码会。