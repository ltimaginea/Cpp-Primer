# *new delete*

在C++中，动态内存的管理是通过一对运算符来完成的： `new` ，在动态内存中为对象分配内存空间并返回一个指向该对象的指针，我们可以选择对对象进行初始化； `delete` ，接受一个动态对象的指针，销毁该对象，并释放与之关联的内存。 

由内置指针（而不是智能指针）管理的动态内存在被显式释放前一直都会存在。

使用new和delete来直接管理内存的类与使用智能指针的类不同，它们不能依赖类对象拷贝、赋值和析构操作的任何默认定义，即必须定义自己版本的拷贝、赋值和析构操作来管理所关联的内存。

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

如果程序耗光了自由空间内存，new表达式就会失败，返回一个空指针，同时它会抛出一个类型为bad_alloc的异常。我们也可以改变使用new的方式来阻止它抛出异常。如果将nothrow传递给new，我们的意图是告诉它不能抛出异常。如果这种形式的new不能分配所需内存，它会返回一个空指针。bad_alloc和nothrow都定义在头文件new中。

```cpp
size_t n = 1024 * 1024 * 1024;

// 如果分配失败，new返回一个空指针
int* pi1 = new int[n];	// 如果分配失败，new抛出std::bad_alloc
delete[] pi1;

// 如果分配失败，new返回一个空指针
int* pi2 = new (nothrow) int[n];
delete[] pi2;
```



## *Tips*

-  `空悬指针` （dangling pointer） 是一个指针，指向曾经保存一个对象但现在已释放的内存。
-  `野指针` （wild pointer）是指未初始化的指针，其指向的位置是不确定的。
- 使用new和delete管理动态内存时常见的问题：
  - 忘记delete内存。忘记释放动态内存会导致人们常说的“内存泄漏”问题，因为这种内存永远不可能被归还给自由空间了。
  - 使用了错误的delete形式。一方面，如果delete释放一块并非new分配的内存，那么其行为是未定义的。另一方面，如果分配了单个对象，则必须使用“delete”；如果分配了数组，则需要用“delete[]”。如果使用了不正确的delete形式，那么结果将是未定义的。在有些平台上，程序会在运行时崩溃。在其他平台上，它会妨碍进一步运行，有时会泄漏资源和破坏内存。
  - 使用空悬指针。通过在释放内存后将指针置为nullptr，可以部分程度上提供保护。当有多个指针指向相同的内存时，在delete内存之后重置指针为nullptr的方法只对这个指针有效，对其他任何仍指向（已释放的）内存的指针是没有作用的。
  - 同一块内存释放多次。当有多个指针指向相同的动态分配对象时，可能发生这种错误。如果对其中一个指针进行了delete操作，对象的内存就被归还给自由空间了。如果我们随后又delete第二个指针，自由空间就可能被破坏。