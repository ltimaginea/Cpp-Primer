# HasPtrPointerLike

对于行为类似指针的类，我们需要为其定义拷贝构造函数和拷贝赋值运算符，来拷贝指针成员本身而不是它指向的string。我们的类仍然需要自己的析构函数来释放接受string参数的构造函数分配的内存。但是，在本例中，析构函数不能单方面地释放关联的string。只有当最后一个指向string的HasPtr销毁时，它才可以释放string。 

**令一个类展现类似指针的行为的最好方法是使用shared_ptr来管理类中的资源**。拷贝（或赋值）一个shared_ptr会拷贝（赋值）shared_ptr所指向的指针。shared_ptr类自己记录有多少用户共享它所指向的对象。当没有用户使用对象时，shared_ptr类负责释放资源。 

但是，有时我们希望直接管理资源。在这种情况下，使用**引用计数**（reference count）就很有用了。为了说明引用计数如何工作，我们将重新定义HasPtr，令其行为像指针一样，但我们不使用shared_ptr，而是设计自己的引用计数。

## 行为像指针的类 *pointer-like*

### 引用计数 

引用计数的工作方式如下：

- 除了初始化对象外，每个构造函数（拷贝构造函数除外）还要创建一个引用计数，用来记录有多少对象与正在创建的对象共享状态。当我们创建一个对象时，只有一个对象共享状态，因此将计数器初始化为1。
- 拷贝构造函数不分配新的计数器，而是拷贝给定对象的数据成员，包括计数器。拷贝构造函数递增共享的计数器，指出给定对象的状态又被一个新用户所共享。
- 析构函数递减计数器，指出共享状态的用户少了一个。如果计数器变为0，则析构函数释放状态。
- 拷贝赋值运算符递增右侧运算对象的计数器，递减左侧运算对象的计数器。如果左侧运算对象的计数器变为0，意味着它的共享状态没有用户了，拷贝赋值运算符就必须销毁状态。 

唯一的难题是确定在哪里存放引用计数。计数器不能直接作为HasPtr对象的成员。下面的例子说明了原因： 

```cpp
HasPtr p1("Hiya!");
HasPtr p2(p1);
HasPtr p3(p1);
```

如果引用计数保存在每个对象中，当创建p3时我们应该如何正确更新它呢？可以递增p1中的计数器并将其拷贝到p3中，但如何更新p2中的计数器呢？ 同时也不能以 static member data 保存引用计数，那样就会导致HasPtr类的所有对象共享一份引用计数了，尤其当通过HasPtr创建出两个无关的对象时，它们却在共享同一份引用计数，这样显然是存在问题的。

解决此问题的一种方法是将计数器保存在动态内存中。当创建一个对象时，我们也分配一个新的计数器。当拷贝或赋值对象时，我们拷贝指向计数器的指针。使用这种方法，副本和原对象都会指向相同的计数器。

### 定义使用引用计数的类

```cpp
// reference counted version of HasPtr
#include <iostream>
#include <string>

class HasPtr
{
public:
	// constructor allocates a new string and a new counter which sets to 1
	HasPtr(const std::string& str = std::string()) :ps_(new std::string(str)), i_(0), use_(new std::size_t(1)) {  }
	// copy constructor copies all three data members and increments the counter
	HasPtr(const HasPtr& rhs) :ps_(rhs.ps_), i_(rhs.i_), use_(rhs.use_) { ++* use_; }
	HasPtr& operator=(const HasPtr&);
	~HasPtr();

	// 为了保证移后源对象是有效的、可析构的状态，借助new表达式勉强实现的移动操作，但这样的实现是可能抛异常的，可以认为是失败的实现
	// move constructor takes over the pointers from its argument and makes the argument safe to destroy
	HasPtr(HasPtr&& rhs) /*noexcept*/ :ps_(rhs.ps_), i_(rhs.i_), use_(rhs.use_) { rhs.ps_ = new std::string(); rhs.use_ = new std::size_t(1); }
	HasPtr& operator=(HasPtr&&) /*noexcept*/;

private:
	std::string* ps_;
	int i_;
	std::size_t* use_;		// member to track how many objects share *ps_
};

HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
	// increment the use count of the right-hand operand
	++* rhs.use_;
	// then decrement this object's counter, if no other users, free this object's allocated members
	if ((-- * use_) == 0)
	{
		delete ps_;
		delete use_;
	}
	// copy data from rhs into this object
	ps_ = rhs.ps_;
	i_ = rhs.i_;
	use_ = rhs.use_;
	// return this object
	return *this;
}

HasPtr::~HasPtr()
{
	// if the reference count goes to 0, delete the string and the counter
	if ((-- * use_) == 0)
	{
		delete ps_;
		delete use_;
	}
}

// 为了保证移后源对象是有效的、可析构的状态，借助new表达式勉强实现的移动操作，但这样的实现是可能抛异常的，可以认为是失败的实现
HasPtr& HasPtr::operator=(HasPtr&& rhs) /*noexcept*/
{
	// direct test for self-assignment
	if (this != &rhs)
	{
		// do the work of the destructor
		if ((-- * use_) == 0)
		{
			delete ps_;
			delete use_;
		}
		// do the work of the move constructor
		ps_ = rhs.ps_;
		i_ = rhs.i_;
		use_ = rhs.use_;
		rhs.ps_ = new std::string();
		rhs.use_ = new std::size_t(1);
	}
	return *this;
}

int main()
{
	HasPtr hp1("Sorry, I am so tired.");
	// no new memory is allocated, hp1 and hp2 share the same underlying string
	HasPtr hp2(hp1);
	HasPtr hp3(hp2);

	HasPtr hp4("Just Courage!");
	hp4 = std::move(hp4);
	hp4 = std::move(hp3);
	HasPtr hp5("Focus on!");
	hp3 = hp5;

	return 0;
}

```

析构函数不能无条件地delete ps_ ——可能还有其他对象指向这块内存。析构函数应该递减引用计数，指出共享string的对象少了一个。如果计数器变为0，则析构函数释放 ps_ 和 use_ 指向的内存。

当拷贝或赋值一个HasPtr对象时，我们希望副本和原对象都指向相同的string。即，当拷贝一个HasPtr时，我们将拷贝 ps_ 本身，而不是 ps_ 指向的string。当我们进行拷贝时，还会递增该string关联的计数器。

拷贝构造函数拷贝给定HasPtr的所有三个数据成员。这个构造函数还递增 use_ 成员，指出 ps_ 和 rhs.ps_ 指向的string又有了一个新的用户。 

拷贝赋值运算符与往常一样执行类似拷贝构造函数和析构函数的工作。即，它必须递增右侧运算对象的引用计数（即，拷贝构造函数的工作），并递减左侧运算对象的引用计数，在必要时释放使用的内存（即，析构函数的工作）。 

而且与往常一样，**赋值运算符必须处理自赋值。我们通过先递增rhs中的计数然后再递减左侧运算对象中的计数来实现这一点**。通过这种方法，当两个对象相同时，在我们检查 ps_ （及 use_ ）是否应该释放之前，计数器就已经被递增过了。

对于移动操作，《C++ Primer》中的这个示例添加移动操作是很勉强的。如果按照《C++ Primer》的源码示例那样实现： [HasPtrUseCount.cc](./Ch00_GreatResources/CppPrimer5e_SourceCode/code_GCC_4_7_0/13/HasPtrUseCount.cc) ，将会导致移后源对象无法析构和无法赋予新值的严重问题，违反了移动操作的要求。表面看到是因为对无效指针解引用导致了问题，其实根本原因在于书中示例类的默认构造函数对指针成员的默认值是`new std::string`，并非是`nullptr`，从而导致了后续一系列的问题。指针成员的默认值是有效指针（有时会是比较昂贵的操作），这样代码后续就没有再对指针成员进行有效性检查。如果指针成员的默认值是`nullptr`，那么示例代码中就会对所有指针成员解引用的操作，首先进行是否空指针的检查，这时然后我们再按照《C++ Primer》的源码示例那样实现移动操作，就是没有问题的了。本文档示例代码中添加的移动操作，虽然对《C++ Primer》书中的示例可以不做修改，[令被移动对象遗留于合法状态](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)，即移后源对象可析构和赋予新值，但是带来的问题在于进行了动态内存分配，导致了移动操作可能抛异常！对书中示例彻底修改后的完美实现就是 `std::shared_ptr` 。



> ## *References*
>
> [令被移动对象遗留于合法状态](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)
>
> [HasPtrUseCount.cc](./Ch00_GreatResources/CppPrimer5e_SourceCode/code_GCC_4_7_0/13/HasPtrUseCount.cc)
>
> 

