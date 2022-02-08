#include <iostream>

class S
{
public:
	S() : n_(10) {  }
	// regardless of whether the members are on the same or different instances
	S(const S& other) : n_(other.n_) {  }	// other.n is accessible in S::S
private:
	int n_;
};

class Derived;

class Base
{
public:
	virtual ~Base() = default;
	void F(Base&, Derived&);
public:
	int public_;
protected:
	int protected_;
private:
	int private_;
};

//class Derived : private Base
//class Derived : protected Base
class Derived : public Base
{
public:
	void Access(Derived& d)
	{
		public_ = 1;			// OK
		protected_ = 1;			// OK，访问从基类继承的 protected 成员
		//private_ = 1;			// 错，不能访问基类私有成员
		d_public_ = 1;			// OK
		d_protected_ = 1;		// OK
		d_private_ = 1;			// OK

		d.public_ = 1;			// OK
		// regardless of whether the members are on the same or different instances
		d.protected_ = 1;		// OK
		//d.private_ = 1;		// 错
		d.d_public_ = 1;		// OK
		// regardless of whether the members are on the same or different instances
		d.d_protected_ = 1;		// OK
		// regardless of whether the members are on the same or different instances
		d.d_private_ = 1;		// OK

		Base f;
		f.public_ = 1;			// OK
		//f.protected_ = 1;		// 错，在派生类的成员函数中不能访问普通的基类对象的保护成员
		//f.private_ = 1;		// 错
	}
public:
	int d_public_;
protected:
	int d_protected_;
private:
	int d_private_;
};

void Base::F(Base& b, Derived& d)
{
	++protected_;

	// regardless of whether the members are on the same or different instances
	++b.private_;
	++b.protected_;
	++b.public_;

	++d.public_;		// 经过测试发现仅公有继承时才可以访问该成员
	++d.protected_;		// 经过测试发现仅公有继承时才可以访问该成员
	++d.private_;		// 经过测试发现仅公有继承时才可以访问该成员
	++d.d_public_;
	//++d.d_protected_;
	//++d.d_private_;
}

// non-member/non-friend
void X(Base& b, Derived& d)
{
	//++b.protected_;	// error: no access from non-member/non-friend
	//++d.protected_;	// error: no access from non-member/non-friend
}

int main()
{
	Base b;
	Derived d;
	//int n = b.protected_;		// 错，在类的成员函数外部，不能访问基类保护成员
	//n = d.private_;			// 错，此处不能访问d的私有成员
	d.public_ = 10;				// 仅公有继承时可以访问
	d.d_public_ = 11;			// OK

	return 0;
}

// Tips:
// 1. 在类的成员函数内部可以访问同类其他对象的全部(私有, 保护, 公有)成员(不含继承的基类的私有成员，其是不可访问的)。
// 2. 需要留意基类Base的成员函数Base::F对普通的派生类对象的成员的访问权限：
// 按理说，基类的成员和友元对于普通的派生类对象，也应该是不具有特殊的访问权限的。
// 但经过测试发现，对于protected或private继承的方式，基类的成员和友元是符合预期地仅可以访问普通派生类对象自定义部分的public成员；
// 但是对于public继承的方式，基类的成员和友元是出乎预期地既可以访问普通派生类对象自定义部分的public成员，还可以访问普通派生类对象继承自基类部分的所有成员。
