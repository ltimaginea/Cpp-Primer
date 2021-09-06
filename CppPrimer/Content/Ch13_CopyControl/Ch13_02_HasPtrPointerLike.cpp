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
