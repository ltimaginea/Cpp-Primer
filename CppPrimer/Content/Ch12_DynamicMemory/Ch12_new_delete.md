# *new delete*

在C++中，动态内存的管理是通过一对运算符来完成的： `new` ，在动态内存中为对象分配内存空间，返回一个指向该对象的指针，我们可以选择对对象进行初始化； `delete` ，接受一个动态对象的指针，销毁该对象（调用析构函数），并释放与之关联的内存（free 内存）。 

使用 `new` 和 `delete` 来直接管理内存的类与使用智能指针的类不同，它们不能依赖类对象拷贝、赋值和析构操作的任何默认定义，即必须定义自己版本的拷贝、赋值和析构操作来管理所关联的内存。

由内置指针（而不是智能指针）管理的动态内存在被显式释放前一直都会存在。

**在使用 `delete` 释放动态内存后，应该将指针置为 `nullptr`** 。注意，当有多个指针指向相同的内存时，在 `delete` 内存之后重置指针为 `nullptr` 的方法只对这个指针有效，对其他任何仍指向（已释放的）内存的指针是没有作用的，虽然这只能部分程度上对程序提供保护，但是总比保持原状要好（下文中的代码示例为了节省篇幅，所以没有将释放内存后的指针置为 `nullptr` ）。

内存泄漏（memory leak）：通常我们认为，在运行的程序中，如果一个无法访问的对象却仍然占用着内存空间，即为此对象造成了内存泄漏。

当程序中通过 `new` 或 `malloc` 申请了一片内存，但没有 `delete` 或 `free` 时，在程序运行结束前，这片内存是一直被占用着的，也就是内存泄漏，但当程序运行结束后，操作系统是会对那片内存进行回收释放的，即当用户进程结束时操作系统会释放该进程所占用的所有空间。

使用内置指针来管理动态内存是复杂而又容易出错的，尤其当异常发生时，详见： [Ch12_new_RawPointer_AvoidMemoryLeak.md](./Ch12_new_RawPointer_AvoidMemoryLeak.md) 。

现代C++程序应尽可能使用智能指针。

## 动态分配和内存释放

在自由空间分配的内存是无名的，因此new无法为其分配的对象命名，而是返回一个指向该对象的指针。

我们传递给delete的指针必须指向动态分配的内存，或者是一个空指针（释放一个空指针总是没有错误的）。释放一块并非new分配的内存，或者将相同的指针值释放多次，其行为是未定义的。

```cpp
int* p0 = nullptr;
// 正确，释放一个空指针总是没有错误的
delete p0;
```

默认情况下，动态分配的对象是默认初始化的，这意味着内置类型或组合类型的对象的值将是未定义的，而类类型对象将用默认构造函数进行初始化。

```cpp
// 默认初始化，*p1的值未定义
int* p1 = new int;
delete p1;
```

我们可以使用直接初始化方式来初始化一个动态分配的对象。我们可以使用传统的构造方式（使用圆括号），在C++11中，也可以使用列表初始化（使用花括号）。

我们也可以对动态分配的对象进行值初始化，只需在类型名之后跟一对空括号即可。

```cpp
// 9999999999
string* p6 = new string(10, '9');
delete p6;

// 使用花括号进行列表初始化
vector<int>* p7 = new vector<int>{ 0,1,2,3,4,5,6,7,8,9 };
delete p7;

// 值初始化，*p2的值为0
int* p2 = new int();
delete p2;
```

一个动态分配的const对象必须进行初始化。对于一个定义了默认构造函数的类类型，其const动态对象可以隐式初始化，而其他类型的对象就必须显式初始化。由于分配的对象是const的，new返回的指针是一个指向const的指针。

```cpp
const int* p8 = new const int(1024);
delete p8;
```

为了让new分配一个对象数组，我们要在类型名之后跟一对方括号`[]` ，在其中指明要分配的对象的数目，new返回一个指向数组首元素的指针。方括号中的大小必须是整型，但不必是常量。 我们也可以用一个表示数组类型的类型别名来分配一个数组，这样，new表达式中就不需要方括号了。

分配一个数组会得到一个数组元素类型的指针，其指向数组首元素。当用new分配一个数组时，我们并未得到一个数组类型的对象，而是得到一个数组元素类型的指针。由于分配的内存并不是一个数组类型，因此不能对动态数组调用begin或end。这些函数使用数组维度（维度是数组类型的一部分）来返回指向首元素和尾后元素的指针。出于相同的原因，也不能用范围for语句来处理动态数组中的元素。

为了释放动态数组，我们使用一种特殊形式的delete——在指针前加上一个空方括号对。当我们使用一个类型别名来定义一个数组类型时，在new表达式中不使用[]。即使是这样，在释放一个数组指针时也必须使用方括号。数组中的元素按逆序销毁，即，最后一个元素首先被销毁，然后是倒数第二个，依此类推。 当我们释放一个指向数组的指针时，空方括号对是必需的：它指示编译器此指针指向一个对象数组的第一个元素。如果我们在delete一个指向数组的指针时忽略了方括号（或者在delete一个指向单一对象的指针时使用了方括号），其行为是未定义的。

默认情况下，new分配的对象，不管是单个分配的还是数组中的，都是默认初始化的。

我们可以对数组中的元素进行值初始化，方法是在大小之后跟一对空括号。

在C++11中，我们还可以提供一个元素初始化器的花括号列表。与内置数组对象的列表初始化一样，初始化器会用来初始化动态数组中开始部分的元素。如果初始化器数目小于元素数目，剩余元素将进行值初始化。如果初始化器数目大于元素数目，则new表达式失败，不会分配任何内存，new会抛出一个类型为bad_array_new_length的异常。

```cpp
// 10个未初始化的int
int* p10 = new int[10];		// 分配一个10个int的数组，p10指向第一个int
delete[] p10;

// 值初始化，10个初始化为0的int
typedef int arrT[10];
int* p11 = new arrT();		// 分配一个10个int的数组，p11指向第一个int
delete[] p11;

// 值初始化，10个空string
string* p12 = new string[10]();
delete[] p12;

// 10个string，前4个用给定的初始化器初始化，剩余的进行值初始化
string* p13 = new string[10]{ "a","an","the",string(7,'x') };
delete[] p13;
```

动态分配一个空数组是合法的，但其指针不能解引用。虽然我们不能创建一个大小为0的静态数组对象，但当n等于0时，调用new[n]是合法的。

当我们用new分配一个大小为0的数组时，new返回一个合法的非空指针。此指针保证与new返回的其他任何指针都不相同。对于零长度的数组来说，此指针就像尾后指针一样，我们可以像使用尾后迭代器一样使用这个指针。我们可以用此指针进行比较操作，可以向此指针加上（或从此指针减去）0，也可以从此指针减去自身从而得到0。但此指针不能解引用——毕竟它不指向任何元素。

```cpp
// 错误！不能定义长度为0的数组
// char arr[0];

// 正确，但cp不能解引用
char* cp = new char[0];
delete[] cp;
```

如果我们提供了一个括号包围的初始化器，就可以使用 `auto` 从此初始化器来推断我们想要分配的对象的类型。但是，由于编译器要用初始化器的类型来推断要分配的类型，只有当括号中仅有单一初始化器时才可以使用auto。注意，我们不能用auto分配数组。

```cpp
// p9是一个double*
auto p9 = new auto(6.28);
// auto p99 = new auto(0, 1, 2);		// 错误，括号中只能有单个初始化器
delete p9;
```

## 内存耗尽

如果程序耗光了自由空间内存，new不能分配所要求的内存空间，new表达式就会失败（不会有返回值），它会抛出一个类型为 `std::bad_alloc` 的异常。

我们也可以改变使用new的方式来阻止它抛出异常。如果将 `std::nothrow` 传递给new，我们的意图是告诉它不能抛出异常。如果这种形式的new不能分配所需内存，它会返回一个空指针。我们称这种形式的new为 `定位new (placement new)` 。

std::bad_alloc和std::nothrow都定义在头文件new中。

```cpp
class Widget { double d[1024 * 1024 * 1024]; /* ... */ };

try
{
	// 如果分配失败，new抛出std::bad_alloc
	Widget* ptr = new Widget;

	try
	{
		//... perform some operations(may throw exceptions)
	}
	catch (...)			// for any exception
	{
		delete ptr;		// clean up
		throw;			// rethrow the exception
	}

	delete ptr;			// clean up on normal end
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



// 如果分配失败，new返回一个空指针
Widget* ptr2 = new (std::nothrow) Widget;
// 这个if...else...的判断可能有效
if (ptr2 == nullptr)
{
	std::cout << "new false" << std::endl;
}
else
{
	// ...
}
delete ptr2;
```

> *Effective C++*
>
> **Nothrow new是一个颇为局限的工具，因为它只适用于内存分配；后继的构造函数调用还是可能抛出异常**。
>
> Nothrow new对异常的强制保证性并不高。要知道，表达式 "new (std::nothrow) Widget" 发生两件事，第一，nothrow 版的 operator new被调用，用以分配足够内存给Widget对象。如果分配失败便返回nullptr指针，一如文档所言。如果分配成功，接下来 Widget构造函数会被调用，而在那一点上所有的筹码便都耗尽，因为Widget构造函数可以做它想做的任何事。它有可能又 new一些内存，而没人可以强迫它再次使用 nothrow new。因此虽然 "new (std::nothrow) Widget" 调用的operator new并不抛掷异常，但Widget构造函数却可能会。如果它真那么做，该异常会一如往常地传播。需要结论吗？结论就是：使用 nothrow new 只能保证operator new不抛掷异常，不保证像 "new (std::nothrow) Widget"这样的表达式绝不导致异常。因此你其实没有运用nothrow new的需要。

## 工作机理

`new expression` 和 `delete expression` 的工作机理大致如下：

当我们使用一条 `new表达式` 时：

```cpp
string* ps = new string("Memory Management");
string* arr = new string[10];
```

实际执行了三步操作。第一步，new表达式调用一个名为 `operator new` （或者 `operator new[]` ）的标准库函数。该函数分配一块足够大的、原始的、未命名的内存空间以便存储特定类型的对象（或者对象的数组）。第二步，编译器运行相应的构造函数以构造这些对象，并为其传入初始值。第三步，对象被分配了空间并构造完成，返回一个指向该对象的指针。近似的伪代码如下：(see also: [new delete - ltimaginea (cnblogs)](https://www.cnblogs.com/ltimaginea/p/15063785.html) )

```cpp
// 分配原始内存
void* memory = operator new(sizeof(string));
// 调用构造函数进行初始化
call string::string("Memory Management") on *memory;
// 类型转换
string* ps = static_cast<string*>(memory);
```

> *Note*:
>
> `new expression` 实际执行的过程中，如果构造函数环节失败（例如抛出了异常），C++语言保证，前面第一步 `operator new` 所分配的内存将会被自动调用合适的 `operator delete` 来释放。详见《深度探索C++对象模型》 Ch6.2节 P256页和如下的链接：
>
> - [new-doesnt-leak-if-ctor-throws, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/freestore-mgmt#new-doesnt-leak-if-ctor-throws) : If an exception occurs during the `Fred` constructor of `p = new Fred()`, the C++ language guarantees that the memory `sizeof(Fred)` bytes that were allocated will automagically be released back to the heap.
>
>   Thus the actual generated code is functionally similar to:
>
>   ```cpp
>   // Original code: Fred* p = new Fred();
>   Fred* p;
>   void* tmp = operator new(sizeof(Fred));
>   try {
>     new(tmp) Fred();  // Placement new
>     p = (Fred*)tmp;   // The pointer is assigned only if the ctor succeeds
>   }
>   catch (...) {
>     operator delete(tmp);  // Deallocate the memory
>     throw;                 // Re-throw the exception
>   }
>   ```
>
>   
>
> - [new expression - cppreference.com](https://en.cppreference.com/w/cpp/language/new) 的 Construction 段落 也有相关的描述： If initialization terminates by throwing an exception (e.g. from the constructor), if new-expression allocated any storage, it calls the appropriate [deallocation function](https://en.cppreference.com/w/cpp/memory/new/operator_delete): [`operator delete`](http://en.cppreference.com/w/cpp/memory/new/operator_delete) for non-array type, [`operator delete[]`](http://en.cppreference.com/w/cpp/memory/new/operator_delete) for array type. 
>
> - [operator delete - C++ Reference (cplusplus.com) ](http://www.cplusplus.com/reference/new/operator%20delete/) 也有相关的描述： The other signatures ((2) and (3)) are never called by a *delete-expression* (the `delete` operator always calls the ordinary version of this function, and exactly once for each of its arguments). These other signatures are only called automatically by a *new-expression* when their object construction fails (e.g., if the constructor of an object throws while being constructed by a *new-expression* with nothrow, the matching operator delete function accepting a nothrow argument is called).



当我们使用一条 `delete表达式` 删除一个动态分配的对象时：

```cpp
delete ps;
delete[] arr;
```

实际执行了两步操作。第一步，对 ps 所指的对象或者 arr 所指的数组中的元素执行对应的析构函数。第二步，编译器调用名为 `operator delete` （或者 `operator delete[]` ）的标准库函数释放内存空间。近似的伪代码如下：(see also: [new delete - ltimaginea (cnblogs)](https://www.cnblogs.com/ltimaginea/p/15063785.html) )

```cpp
// 调用析构函数
ps->~string();
// 释放内存空间
operator delete(ps);
```

对于继承体系中的类类型，当我们 `delete` 一个动态分配的指向派生类对象的基类指针时，如前所述， `delete expression` 实际执行了两步操作：第一步是执行对象的析构函数，只要基类的析构函数是虚函数，就能确保当我们 `delete` 基类指针时将运行正确的析构函数版本；第二步调用名为 `operator delete` 的标准库函数释放内存空间，当该指针是指向派生类对象的基类指针时，依然会正确无误地完整释放内存空间，因为一个内存块（block）的头部和脚部编码了这个块的大小和这个块是已分配还是空闲的信息，释放内存时，根据大小和“分配/空闲”标志位的信息就可以完成释放内存空间的操作，一般只需将块头部和脚部的“分配/空闲”标志位由“已分配”设置为“空闲的”即可，所以释放内存空间操作与该指针的动态类型没有关系，不同编译器的具体策略可能会略有不同（详见《C++ Primer》Ch19.1节 和《深入理解计算机系统》 Ch9.9节）。同时，我们从释放内存空间操作的相关函数 [ `void operator delete(void* ptr)` ](https://en.cppreference.com/w/cpp/memory/new/operator_delete) 和 [ `void free(void* ptr)` ](https://en.cppreference.com/w/cpp/memory/c/free) 的函数签名可以发现，函数的参数类型是 `void*` ，这也从另一方面说明，释放内存空间操作与指针类型是无关的。注意，以上的讨论不包括数组，绝对不要以多态方式处理数组！原因详见《深度探索C++对象模型》Ch6.2 节，《More Effective C++》Item3 以及 C++ 核心指南 C.152 : [C.152: Never assign a pointer to an array of derived class objects to a pointer to its base (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-array) 。



## Tips

-  `空悬指针` （dangling pointer） 是一个指针，指向曾经保存一个对象但现在已释放的内存。
-  `野指针` （wild pointer）是指未初始化的指针，其指向的位置是不确定的。
-  `内存泄漏` （memory leak）是指在运行的程序中，如果一个无法访问的对象却仍然占用着内存空间，即为此对象造成了内存泄漏。
- 使用new和delete管理动态内存时常见的问题：
  - 忘记delete内存。忘记释放动态内存会导致人们常说的“内存泄漏”问题，因为这种内存永远不可能被归还给自由空间了。
  - 使用了错误的delete形式。一方面，如果delete释放一块并非new分配的内存，那么其行为是未定义的。另一方面，如果分配了单个对象，则必须使用“delete”；如果分配了数组，则需要用“delete[]”。如果使用了不正确的delete形式，那么结果将是未定义的。在有些平台上，程序会在运行时崩溃。在其他平台上，它会妨碍进一步运行，有时会泄漏资源和破坏内存。
  - 使用空悬指针。通过在释放内存后将指针置为nullptr，可以部分程度上提供保护。当有多个指针指向相同的内存时，在delete内存之后重置指针为nullptr的方法只对这个指针有效，对其他任何仍指向（已释放的）内存的指针是没有作用的。
  - 同一块内存释放多次，会发生 double free 错误。通过在释放内存后将指针置为nullptr，可以部分程度上提供保护。因为当有多个指针指向相同的动态分配对象时，依然可能会发生 double free 错误：如果对其中一个指针进行了 delete 操作，对象的内存就被归还给自由空间了，如果我们随后又 delete 第二个指针，自由空间就可能被破坏，即会发生 double free 错误。



## References

- [Memory Management, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/freestore-mgmt)
- [new expression - cppreference.com](https://en.cppreference.com/w/cpp/language/new)
- [operator delete - C++ Reference (cplusplus.com) ](http://www.cplusplus.com/reference/new/operator%20delete/)
- [operator delete, operator delete[] - cppreference.com](https://en.cppreference.com/w/cpp/memory/new/operator_delete)
- [new delete_ltimaginea的博客-CSDN博客](https://blog.csdn.net/sinat_43125576/article/details/121015742)
- [new delete - ltimaginea - 博客园 (cnblogs.com)](https://www.cnblogs.com/ltimaginea/p/15063785.html)

