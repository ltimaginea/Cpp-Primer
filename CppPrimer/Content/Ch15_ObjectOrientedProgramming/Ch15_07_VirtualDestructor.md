# Virtual Destructor

**多态基类应该定义一个 `virtual` 析构函数**。这样我们就能动态分配继承体系中的对象了。

我们通过在基类中将析构函数定义成虚函数以确保当我们 `delete` 一个动态分配的对象的指针时将执行正确的析构函数版本。如果基类的析构函数不是虚函数，则 `delete` 一个指向派生类对象的基类指针将产生未定义的行为。

析构函数的虚属性也会被继承。因此，无论派生类使用合成的析构函数还是定义自己的析构函数，都将是虚析构函数。只要基类的析构函数是虚函数，就能确保当我们 `delete` 基类指针时将运行正确的析构函数版本。

一般来说，如果派生类的数据成员没有进行额外的动态内存分配，那么即使基类的析构函数没有定义为虚函数，当 `delete` 一个指向派生类对象的基类指针，调用的是基类的析构函数时，其实一般是不会发生内存泄漏的（经过测试，MSVC和g++不会发生内存泄漏，具体是否会内存泄漏依赖于具体操作系统和对应编译器的实现）。原因具体见 [Ch12_new_delete.md##Explanation](../Ch12_DynamicMemory/Ch12_new_delete.md#explanation) ： `delete expression` 实际执行了两步操作：第一步是执行对象的析构函数。当 `delete` 一个指向派生类对象的基类指针时，程序正确的行为应该是：运行具体派生类的析构函数，基类的析构函数会在派生类的析构函数运行结束之后被自动调用。如果派生类的数据成员没有进行额外的动态内存分配，则派生类的析构函数中就不存在释放数据成员额外分配的动态内存这样的操作，那么即使基类的析构函数没有定义为虚函数，当 `delete` 一个指向派生类对象的基类指针，调用的是基类的析构函数时，在此第一步中是不存在内存泄漏的；第二步调用名为 `operator delete` 的标准库函数释放内存空间，当该指针是指向派生类对象的基类指针时，依然会正确无误地完整释放内存空间。因为一个指向派生类对象的基类指针，仅仅是指针类型由最初的派生类指针类型转换为了基类指针类型，即仅仅是指针类型有所改变，但指针的值是没有改变的，通过指针的值便可以寻找到最初动态分配的内存块（block）。一个内存块的头部和脚部编码了这个块的大小和这个块是已分配还是空闲的信息，释放内存时，根据大小和“分配/空闲”标志位的信息就可以完成释放内存空间的操作，一般只需将块头部和脚部的“分配/空闲”标志位由“已分配”设置为“空闲的”即可，所以释放内存空间操作与该指针的动态类型没有关系，即在第二步中也是不存在内存泄漏的，不同编译器的具体策略可能会略有不同（详见《C++ Primer》Ch19.1节 和《深入理解计算机系统》 Ch9.9节）。同时，我们从释放内存空间操作的相关函数 [`void operator delete(void* ptr)` ](https://en.cppreference.com/w/cpp/memory/new/operator_delete)和 [`void free(void* ptr)` ](https://en.cppreference.com/w/cpp/memory/c/free)的函数签名可以发现，函数的参数类型是 `void*` ，这也从另一方面说明，释放内存空间操作与指针类型是无关的。注意，以上的讨论不包括数组，绝对不要以多态方式处理数组！原因详见 [Ch15_08_Never_Polymorphism_Array.md](./Ch15_08_Never_Polymorphism_Array.md) ，《More Effective C++》Item3 以及 C++ 核心指南 C.152 : [C.152: Never assign a pointer to an array of derived class objects to a pointer to its base (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-array) 。注意！以上的论述仅在于解释“如果派生类的数据成员没有进行额外的动态内存分配，那么即使基类的析构函数没有定义为虚函数，当 `delete` 一个指向派生类对象的基类指针，调用的是基类的析构函数时，其实一般是不会发生内存泄漏的”该现象的原因，但是即使没有发生内存泄漏，它也是不符合C++标准的，算是钻了底层实现的空子，要杜绝这样的写法。

## Copy Control

**虚析构函数将阻止合成移动操作**。如果一个类显式定义了虚析构函数，即使它通过 `=default` 的形式使用了合成的版本，编译器也不会为这个类合成移动操作。因此，应该遵循 [*The rule of three/five/zero*](../Ch13_CopyControl/Ch13_01_The_rule_of_three_five_zero.md) 其中的建议：显式定义多态基类的所有五个拷贝控制成员，同时将基类的 copy 和 move 操作定义为 `protected` 以防止 slicing ，并且使得只有派生类能够在自己的 copy 和 move 操作中调用它们。

## Example

```cpp
#include <iostream>
#include <string>
#include <memory>

class B
{
public:
	B(int b = 0) : b_(b) {  }
	virtual ~B() = default;
	virtual void ClassSize() { std::cout << "sizeof(B) = " << sizeof(B) << std::endl; }
protected:
	// Make the polymorphic base class copy/move protected to prevent the slicing, 
	// and so that only derived classes can invoke them in their own copy/move.
	B(const B&) = default;
	B(B&&) = default;
	B& operator=(const B&) = default;
	B& operator=(B&&) = default;
private:
	int b_;
};

class D1 final : public B
{
public:
	D1(int b = 1, std::string str = std::string("default")) : B(b), str_(str) {  }
	D1(const D1&) = default;
	D1(D1&&) = default;
	D1& operator=(const D1&) = default;
	D1& operator=(D1&&) = default;
	~D1() override = default;
	void ClassSize() override { std::cout << "sizeof(D1) = " << sizeof(D1) << std::endl; }
private:
	std::string str_;
};

class D2 final : public B
{
public:
	D2(int b = 2, int i = 911) : B(b), ptr_(std::make_unique<int>(i)) {  }
	D2(const D2& other) : B(other), ptr_(other.ptr_ ? std::make_unique<int>(*other.ptr_) : nullptr) {  }
	D2(D2&&) = default;
	D2& operator=(const D2& other) { B::operator=(other); ptr_ = other.ptr_ ? std::make_unique<int>(*other.ptr_) : nullptr; return *this; }
	D2& operator=(D2&&) = default;
	~D2() override = default;
	void ClassSize() override { std::cout << "sizeof(D2) = " << sizeof(D2) << std::endl; }
private:
	std::unique_ptr<int> ptr_;
};

int main()
{
	std::unique_ptr<B> b_ptr1 = std::make_unique<D1>(111, "A class with a virtual function should have a virtual or protected destructor");
	std::unique_ptr<B> b_ptr2 = std::make_unique<D2>(222, 110);

	return 0;
}

```



## [Discussion: Make base class destructors public and virtual, or protected and non-virtual (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Sd-dtor)

In rarer cases, such as policy classes, the class is used as a base class for convenience, not for polymorphic behavior. **The destructors of these classes should be made protected and non-virtual.** If the destructor is protected, then calling code cannot destroy through a base class pointer and the destructor does not need to be virtual; it does need to be protected, not private, so that derived destructors can invoke it. So, in these rarer cases, destructors should be protected so that only derived classes can invoke it in their own destructors, and non-virtual since it doesn't need to behave virtually.

Example1: 

```cpp
class My_policy {
public:
    virtual ~My_policy();      // BAD, public and virtual
protected:
    ~My_policy();              // GOOD, protected and non-virtual
    // ...
};

template<class Policy>
class customizable : Policy { /* ... */ }; // note: private inheritance
```



Example2: [boost/core/noncopyable.hpp - 1.78.0](https://www.boost.org/doc/libs/1_78_0/boost/core/noncopyable.hpp) and [noncopyable - 1.78.0 (boost.org)](https://www.boost.org/doc/libs/1_78_0/libs/core/doc/html/core/noncopyable.html) 

```cpp
namespace noncopyable_  // protection from unintended ADL
{
#ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED
#define BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

// noncopyable derives from base_token to enable Type Traits to detect
// whether a type derives from noncopyable without needing the definition
// of noncopyable itself.
//
// The definition of base_token is macro-guarded so that Type Trais can
// define it locally without including this header, to avoid a dependency
// on Core.

  struct base_token {};

#endif // #ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

  class noncopyable: base_token
  {
  protected:
#if !defined(BOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(BOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS)
      BOOST_CONSTEXPR noncopyable() = default;
      ~noncopyable() = default;
#else
      noncopyable() {}
      ~noncopyable() {}
#endif
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS)
      noncopyable( const noncopyable& ) = delete;
      noncopyable& operator=( const noncopyable& ) = delete;
#else
  private:  // emphasize the following members are private
      noncopyable( const noncopyable& );
      noncopyable& operator=( const noncopyable& );
#endif
  };
}
```

```cpp
// noncopyable usage example
#include <boost/core/noncopyable.hpp>

class X : private boost::noncopyable	// note: private inheritance
{
};
```



## References

- [C.127: A class with a virtual function should have a virtual or protected destructor (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dtor)
- [C.35: A base class destructor should be either public and virtual, or protected and non-virtual (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-dtor-virtual)
- [Discussion: Make base class destructors public and virtual, or protected and non-virtual (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Sd-dtor)
- [boost/core/noncopyable.hpp - 1.78.0](https://www.boost.org/doc/libs/1_78_0/boost/core/noncopyable.hpp)
- [noncopyable - 1.78.0 (boost.org)](https://www.boost.org/doc/libs/1_78_0/libs/core/doc/html/core/noncopyable.html)
- *Effective C++* Item07 Declare destructors virtual in polymorphic base classes
- *C++ Coding Standards: 101 Rules, Guidelines, and Best Practices* Item50 Make base class destructors public and virtual, or protected and non-virtual
