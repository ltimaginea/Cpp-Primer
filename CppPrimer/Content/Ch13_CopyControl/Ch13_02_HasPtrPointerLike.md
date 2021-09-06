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







> ## *References*
>
> [令被移动对象遗留于合法状态](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)
>
> [Cpp-Primer/HasPtrUseCount.cc at main · ltimaginea/Cpp-Primer · GitHub](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/code_GCC_4_7_0/13/HasPtrUseCount.cc)
>
> 

