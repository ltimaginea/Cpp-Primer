# Access Control and Inheritance

## Access Specifiers

公有成员（public member） ：能够被类的所有用户访问。

保护成员（protected member） ：能够被派生类的成员和友元访问（Tip: regardless of whether the members are on the same or different instances.） ：

- 保护成员对于派生类的成员和友元来说是可访问的，但对于类的普通用户来说是不可访问的。
- 派生类的成员和友元只能访问派生类对象中的基类部分的受保护成员；对于普通的基类对象中的成员不具有特殊的访问权限。（Tip: 但是，基类的成员和友元对于普通的派生类对象，既可以访问派生类继承自基类部分的所有成员，还可以访问派生类自定义部分的 `public` 成员，见本文最后的示例程序。）

私有成员（private member） ：只能被类的成员和友元访问（Tip: regardless of whether the members are on the same or different instances.）。

`public` ， `protected` 和 `private` 三类关键字在类中出现的次数和先后次序都没有限制。成员的可访问性由离它前面最近的那个访问说明符决定。如果某个成员前面没有访问说明符，则对 class 来说，该成员默认是私有成员；对 struct 来说，该成员默认是公有成员。

## Access Control with Inheritance



我们可以认为一个类有两种不同的用户：普通用户（user）和类的实现者（author）。普通用户编写的代码使用类的对象；类的实现者则负责编写类的成员和友元的代码。

- 普通用户：不考虑继承的话，普通用户只能访问类的公有（接口）成员；考虑继承的话，普通用户仍旧只能访问派生类的公有（接口）成员。
- 类的实现者：不考虑继承的话，类的实现者可以访问类的所有成员；考虑继承的话，派生类的实现者既可以访问派生类自定义部分的所有成员，还可以访问继承自基类部分的 `protected` 成员和 `public` 成员。（Tip: regardless of whether the members are on the same or different instances.）

> *Cpp Primer*
>
> 关键概念：类的设计与受保护的成员 
>
> 不考虑继承的话，我们可以认为一个类有两种不同的用户：普通用户和类的实现者。其中，普通用户编写的代码使用类的对象，这部分代码只能访问类的公有（接口）成员；实现者则负责编写类的成员和友元的代码，成员和友元既能访问类的公有部分，也能访问类的私有（实现）部分。 
>
> 如果进一步考虑继承的话就会出现第三种用户，即派生类。基类把它希望派生类能够使用的部分声明成受保护的。普通用户不能访问受保护的成员，而派生类及其友元仍旧不能访问私有成员。 
>
> 和其他类一样，基类应该将其接口成员声明为公有的；同时将属于其实现的部分分成两组：一组可供派生类访问，另一组只能由基类及基类的友元访问。对于前者应该声明为受保护的，这样派生类就能在实现自己的功能时使用基类的这些操作和数据；对于后者应该声明为私有的。



示例程序：（需要留意基类 Base 的成员函数 Base::F 对普通的派生类对象的成员的访问权限）

```cpp
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
	void F(Base&, Derived&);
public:
	int public_;
protected:
	int protected_;
private:
	int private_;
};

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

	++d.public_;		// OK!!!
	++d.protected_;		// OK!!!
	++d.private_;		// OK!!!
	++d.d_public_;		// OK!!!
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
	d.public_ = 10;				// OK

	return 0;
}

// Tips:
//	1. 在类的成员函数内部可以访问同类其他对象的全部(私有, 保护, 公有)成员(不含基类的私有成员，其是不可访问的)。
//	2. 需要留意基类 Base 的成员函数 Base::F 对普通的派生类对象的成员的访问权限。

```





## References

- [Access specifiers - cppreference.com](https://en.cppreference.com/w/cpp/language/access)
- [Derived classes - cppreference.com](https://en.cppreference.com/w/cpp/language/derived_class)
- [类成员的访问范围说明符_ltimaginea的博客-CSDN博客](https://blog.csdn.net/sinat_43125576/article/details/109106282)
- [不同派生方式下基类成员在派生类中的可访问范围属性_ltimaginea的博客-CSDN博客](https://blog.csdn.net/sinat_43125576/article/details/109250948)
- [虚函数的访问权限_ltimaginea的博客-CSDN博客](https://blog.csdn.net/sinat_43125576/article/details/110359051)

