# Special Member Functions

当定义一个类时，我们显式地或隐式地指定在此类型的对象拷贝、移动、赋值和销毁时做什么。一个类通过定义五种特殊的成员函数来控制这些操作，包括：拷贝构造函数（copy constructor）、拷贝赋值运算符（copy assignment operator）、移动构造函数（move constructor）、移动赋值运算符（move assignment operator）和析构函数（destructor）。拷贝和移动构造函数定义了当用同类型的另一个对象初始化本对象时做什么。拷贝和移动赋值运算符定义了将一个对象赋予同类型的另一个对象时做什么。析构函数定义了当此类型对象销毁时做什么。我们称这些操作为拷贝控制操作（copy control）。

特殊成员函数包括默认构造函数（default constructor）和五个拷贝控制成员。

| Special Member Functions | Guidelines                                                   |
| :----------------------- | :----------------------------------------------------------- |
| default constructor      |                                                              |
| copy constructor         |                                                              |
| move constructor         | mark `noexcept` if it doesn't throw an exception             |
| copy assignment operator | gracefully handle self-assignment and exception safety       |
| move assignment operator | gracefully handle self-assignment and mark `noexcept` if it doesn't throw an exception |
| destructor               | explicitly declare a public and `virtual` destructor in the polymorphic base class |

Example: [Ch15_07_DerivedClassCopyControlMember.cpp](../Ch15_ObjectOrientedProgramming/Ch15_07_DerivedClassCopyControlMember.cpp) 

## copy constructor

拷贝构造函数（copy constructor） 是一种构造函数，将新对象初始化为同类型另一个对象的副本。当向函数传递对象，或以传值方式从函数返回对象时，会隐式使用拷贝构造函数。如果我们未提供拷贝构造函数，编译器会为我们合成一个。

如果一个构造函数的第一个参数是自身类类型的引用（通常是const的），且任何额外参数都有默认值，则此构造函数是拷贝构造函数。

拷贝构造函数的第一个参数必须是一个引用类型。拷贝构造函数被用来初始化非引用类类型参数，这一特性解释了为什么拷贝构造函数自己的参数必须是引用类型。如果其参数不是引用类型，则调用永远也不会成功——为了调用拷贝构造函数，我们必须拷贝它的实参，但为了拷贝实参，我们又需要调用拷贝构造函数，如此无限循环。

如果我们没有为一个类定义拷贝构造函数，编译器会为我们定义一个。与合成默认构造函数不同，即使我们定义了其他构造函数，编译器也会为我们合成一个拷贝构造函数。对某些类来说，合成拷贝构造函数（synthesized copy constructor）用来阻止我们拷贝该类类型的对象。而一般情况，合成的拷贝构造函数会将其参数的成员逐个拷贝到正在创建的对象中。编译器从给定对象中依次将每个非 static 成员拷贝到正在创建的对象中。 每个成员的类型决定了它如何拷贝：对类类型的成员，会使用其拷贝构造函数来拷贝；内置类型的成员则直接拷贝。虽然我们不能直接拷贝一个数组，但合成拷贝构造函数会逐元素地拷贝一个数组类型的成员。如果数组元素是类类型，则使用元素的拷贝构造函数来进行拷贝。

拷贝构造函数在几种情况下都会被隐式地使用。因此，拷贝构造函数通常不应该是 `explicit` 的。发生隐式转换的一种情况是当我们执行拷贝形式的初始化时（使用 `=` ）。当我们用 `explicit` 关键字声明构造函数时，它将只能以直接初始化的形式使用。而且，编译器将不会在自动转换过程中使用该构造函数。原因详见《C++ Primer》7.5.4节和13.1.1 节。

举例： [Ch13_02_HasPtrValueLike_better.cpp](./Ch13_02_HasPtrValueLike_better.cpp) 

## copy assignment operator

拷贝赋值运算符（copy assignment operator） 是接受一个本类型对象的赋值运算符版本。通常，拷贝赋值运算符的参数是一个const的引用，并返回指向本对象的引用。

**拷贝赋值运算符既应该正确处理自赋值的情况，还应该是异常安全的**。C++ 标准库所提供的容器、智能指针等组件都是可以正确处理自赋值的，所以如果一个类的数据成员均是标准库组件和简单内置类型时，我们可以使用 `=default` 来由编译器为我们合成拷贝控制成员。但是如果一个类含有某些特殊类型（如原始指针类型）的数据成员时，我们则需要显式地自定义拷贝控制成员。**当一个类需要我们自定义 copy assignment operator 时，虽然可以通过精心设计代码的逻辑来做到自赋值安全而不需要显式地写出自赋值判断语句 `if (this != &rhs)` ，但是仍推荐显式地写出自赋值判断语句，因为这样会使程序清晰易懂，也便于后期维护，至于一条自赋值判断语句对程序效率的影响，是微不足道的**。关于我们自定义的 copy assignment operator 如何精心设计代码的逻辑来做到自赋值安全而不需要显式地写出自赋值判断语句见 (1) [Ch13_02_HasPtrValueLike.md](./Ch13_02_HasPtrValueLike.md) (2) [Assignment Operators, C++ FAQ (isocpp.org)](https://isocpp.org/wiki/faq/assignment-operators) 。

与处理拷贝构造函数一样，如果一个类未定义自己的拷贝赋值运算符，编译器会为它生成一个合成拷贝赋值运算符（synthesized copy assignment operator）。类似拷贝构造函数，对于某些类，合成拷贝赋值运算符用来禁止该类型对象的赋值。如果拷贝赋值运算符并非出于此目的，它会将右侧运算对象的每个非 static 成员赋予左侧运算对象的对应成员，这一工作是通过成员类型的拷贝赋值运算符来完成的。对于数组类型的成员，逐个赋值数组元素。合成拷贝赋值运算符返回一个指向其左侧运算对象的引用。

举例： [Ch13_02_HasPtrValueLike_better.cpp](./Ch13_02_HasPtrValueLike_better.cpp) 

## move semantics

详见： [Ch13_06_MoveSemantics.md](./Ch13_06_MoveSemantics.md) 

## destructor

详见： [Ch13_01_destructor.md](./Ch13_01_destructor.md) 

## synthesized copy control

详见： [Ch13_00_CopyControl.md](./Ch13_00_CopyControl.md) 

## the rule of three/five/zero

详见： [Ch13_01_The_rule_of_three_five_zero.md](./Ch13_01_The_rule_of_three_five_zero.md)

