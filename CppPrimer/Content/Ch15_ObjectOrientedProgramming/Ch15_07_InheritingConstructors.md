# Inheriting constructors

在C++11新标准中，派生类能够重用其直接基类定义的构造函数。尽管如我们所知，这些构造函数并非以常规的方式继承而来，但是为了方便，我们不妨姑且称其为“继承”的。一个类只初始化它的直接基类，出于同样的原因，一个类也只继承其直接基类的构造函数。类不能继承默认、拷贝和移动构造函数。

派生类继承基类构造函数的方式是提供一条注明了（直接）基类名的using声明语句。举例： `using Base::Base;` 。

通常情况下，using声明语句只是令某个名字在当前作用域内可见。而当作用于构造函数时，using声明语句将令编译器产生代码。对于基类的每个构造函数，编译器都生成一个与之对应的派生类构造函数。换句话说，对于基类的每个构造函数，编译器都在派生类中生成一个形参列表完全相同的构造函数。

举例：

```cpp
class Base
{
public:
	virtual ~Base() = default;

	Base(int i) : i_(i) {  }
	Base(int i, double d) : i_(i), d_(d) {  }
	Base(int i, double d, std::string s) : i_(i), d_(d), s_(s) {  }
private:
	int i_;
	double d_;
	std::string s_;
};

class Derived : public Base
{
public:
	~Derived() override = default;
	
	// inheriting constructors
	using Base::Base;
	/* inheriting constructors are equivalent to:
	Derived(int i) : Base(i) {  }
	Derived(int i, double d) : Base(i, d) {  }
	Derived(int i, double d, std::string s) : Base(i, d, s) {  }
	*/

};
```



如果派生类含有自己的数据成员，则这些成员将被默认初始化。

## 继承的构造函数的特点

和普通成员的using声明不一样，一个构造函数的using声明不会改变该构造函数的访问级别。例如，不管using声明出现在哪儿，基类的私有构造函数在派生类中还是一个私有构造函数；受保护的构造函数和公有构造函数也是同样的规则。 

而且，一个using声明语句不能指定 `explicit` 或 `constexpr` 。如果基类的构造函数是 `explicit` 或者 `constexpr` ，则继承的构造函数也拥有相同的属性。 

当一个基类构造函数含有默认实参时，这些实参并不会被继承。相反，派生类将获得多个继承的构造函数，其中每个构造函数分别省略掉一个含有默认实参的形参。例如，如果基类有一个接受两个形参的构造函数，其中第二个形参含有默认实参，则派生类将获得两个构造函数：一个构造函数接受两个形参（没有默认实参），另一个构造函数只接受一个形参，它对应于基类中最左侧的没有默认值的那个形参。 

如果基类含有几个构造函数，则除了两个例外情况，大多数时候派生类会继承所有这些构造函数。第一个例外是派生类可以继承一部分构造函数，而为其他构造函数定义自己的版本。如果派生类定义的构造函数与基类的构造函数具有相同的参数列表，则该构造函数将不会被继承。定义在派生类中的构造函数将替换继承而来的构造函数。 

第二个例外是默认、拷贝和移动构造函数不会被继承。这些构造函数按照正常规则被合成。继承的构造函数不会被作为用户定义的构造函数来使用，因此，如果一个类只含有继承的构造函数，则它也将拥有一个合成的默认构造函数。



示例程序见：

-  [Ch15_07_InheritingConstructors.cpp](./Ch15_07_InheritingConstructors.cpp)
-  [Observer.cpp#L43](https://github.com/downdemo/Design-Patterns-in-Cpp17/blob/fc5faa91d77a814f11e111f00a25f8118f9ae84e/src/Observer.cpp#L43)
-  [Observer.cpp#L54](https://github.com/downdemo/Design-Patterns-in-Cpp17/blob/fc5faa91d77a814f11e111f00a25f8118f9ae84e/src/Observer.cpp#L54)

## References

- [Using-declaration - cppreference.com](https://en.cppreference.com/w/cpp/language/using_declaration)

