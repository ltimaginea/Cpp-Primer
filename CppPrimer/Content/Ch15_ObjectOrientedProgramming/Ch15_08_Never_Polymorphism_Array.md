# Never Polymorphism Array

```cpp
#include <iostream>
#include <string>

class Base
{
public:
	virtual ~Base() = default;
	virtual void f() { std::cout << "Base" << std::endl; }
	int b_ = 1;
};

class Derived : public Base
{
public:
	~Derived() override = default;
	void f() override { std::cout << "Derived" << std::endl; }
	double d_ = 3.14;
	std::string str_ = "Derived";
};

int main()
{
	constexpr int kSize = 10;

	Derived d[kSize];
	// Evil: d decays to &d[0] which is converted to Base*
	Base* ptr1 = d;
	for (int i = 0; i < kSize; ++i)
	{
		// Error: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.
		std::cout << ptr1[i].b_ << std::endl;
		// Error: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.
		ptr1[i].b_ = 2;		// lead to memory corruption
		// Error: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.
		ptr1[i].f();
	}

	// Evil!
	Base* ptr2 = new Derived[kSize];
	// Error: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.
	delete[] ptr2;
	ptr2 = nullptr;

	return 0;
}

```



数组的可调整性很差：多态地处理数组是绝对的类型错误，而且编译器有可能不会做出任何提示。数组对象几乎总是会涉及指针的算术运算，在多态情形下，指针的算术运算通常会是不正确的，所以多态和指针的算术运算不能混用。

对于上文示例程序中的 `ptr1` ，在 for 循环中自从数组第一个元素之后的指针算术运算都将会发生错误。因为在对指向 Derived 对象的 Base 指针 `ptr1` 进行指针算术运算时，`ptr1[i]` 代表的其实是 `*(ptr1 + i)` ，编译器会将其本质上计算为 `*(ptr1 + i * sizeof(Base))` ，即是将内存中的对象都当作 Base 类型的了——而不是实际的大小可能不同的 Derived 类型。由于 derived classes 通常比其 base classes 有更多的  data members ，所以 derived class objects 通常比其 base class objects 要大。在这样的情形下，`ptr1[i]` 的地址很有可能不位于实际对应的 Derived 类型对象的起始位置，即这时的指针算术运算会是不正确的。

对于上文示例程序中的 `ptr2` ，一方面对其进行指针算术运算时将会产生错误，另一方面在 `delete[]` 此指针时，即在通过 base class 指针删除一个由 derived classes objects 构成的数组时，也将会发生错误。因为当数组被删除时，编译器需要借助指针的算术运算来对数组中每一个元素进行调用 destructor ，因此结果会出错。如果迫不得已写这样的程序，那么解决方法是先将指针转换为原先的派生类指针，然后再进行 `delete[]` ，代码如下：

```cpp
Derived* p = dynamic_cast<Derived*>(ptr2);
delete[] p;		// correct!
```

## Solution

这种错误是可替换性（规定指向派生类的指针可用作指向其基类的指针）和C语言遗留的指针运算（认为指针是单态的，只使用静态信息计算跨距）之间糟糕的交互作用引起的。 

要存储多态对象的数组，需要用到一个基类指针（例如原始指针，或更好的 `std::shared_ptr` ）的数组（使用真正的容器更好），这样数组中每个指针都指向一个多态对象（很可能分配在自由存储区中），举例： `std::vector<std::shared_ptr<Base>> basket;` 。示例程序见：

现代C++中要避免使用C风格数组，应该优先使用 `std::array` 或智能指针，因为它们从语法上就禁止了多态地处理数组：

```cpp
std::array<Derived, kSize> arr_d1;
// Compile Error: C++ does not allow this:
std::array<Base, kSize> arr_b = arr_d1;
// Ok: C++ only allows this:
std::array<Derived, kSize> arr_d2 = arr_d1;

// Compile Error: C++ does not allow this:
std::unique_ptr<Base[]> ptr3(new Derived[kSize]);
// Ok: C++ only allows this:
std::unique_ptr<Derived[]> ptr4(new Derived[kSize]);
```

## References

- *More Effective C++* Item3 绝对不要以多态方式处理数组
- *C++ Coding Standards: 101 Rules, Guidelines, and Best Practices* 第100条 不要多态地处理数组
- [C.152: Never assign a pointer to an array of derived class objects to a pointer to its base (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-array)
- [Is an array of Derived a kind-of array of Base?, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/proper-inheritance#array-derived-vs-base)
- [Does array-of-Derived is-not-a-kind-of array-of-Base mean arrays are bad?, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/proper-inheritance#arrays-are-covariant)

