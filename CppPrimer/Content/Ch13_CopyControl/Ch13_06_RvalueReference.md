# rvalue reference

## 左值和右值

`表达式` 由一个或多个运算对象（operand）组成，对表达式求值将得到一个结果（result）。字面值和变量是最简单的表达式（expression），其结果就是字面值和变量的值。把一个运算符（operator）和一个或多个运算对象组合起来可以生成较复杂的表达式。

[Value categories - cppreference.com](https://en.cppreference.com/w/cpp/language/value_category) : Each C++ expression (an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties: a *type* and a *value category*. Each expression has some non-reference type, and each expression  belongs to exactly one of the three primary value categories: *prvalue*, *xvalue*, and *lvalue*.

**左值和右值是表达式的属性，该属性会决定其可以被绑定到左值引用还是右值引用上：一个左值只能被绑定到左值引用上，一个右值可以被绑定到右值引用或者const的左值引用上**。C++的表达式要不然是右值，要不然就是左值。

`左值 (lvalue)` 是指那些求值结果为对象或函数的表达式。**一个表示对象的非常量左值可以作为赋值运算符的左侧运算对象**。

`右值 (rvalue)` 是指一种表达式，其结果是值而非值所在的位置。**右值只能作为赋值运算符的右侧运算对象**。

一般而言，一个左值表达式表示的是一个对象的身份（在内存中的位置），而一个右值表达式表示的是对象的值（内容）。

返回左值引用的函数，连同赋值、下标、解引用和前置递增/递减运算符，都是返回左值的表达式的例子。**一个左值引用可以绑定到这类左值表达式的结果上**。 

返回非引用类型的函数，连同算术、关系、位以及后置递增/递减运算符，都生成右值。**一个右值引用只能绑定到这类右值表达式上。我们不能将一个左值引用绑定到这类右值表达式上，但我们可以将一个const的左值引用绑定到这类右值表达式上**。

左值有持久的状态，而右值要么是字面常量（除了字符串字面量），要么是在表达式求值过程中创建的临时对象。

## 右值引用

`右值引用 (rvalue reference)` 是C++11引入的一种新的引用类型，旨在解决非必要的拷贝和实现完美转发。当赋值运算符的右侧是一个右值时，赋值运算符左侧的对象可以去“窃取”右侧右值的资源，而不是执行单独的动态分配，从而实现移动语义。

由于右值引用只能绑定到临时对象（右值），我们可知：（1）所引用的对象将要被销毁。（2）该对象没有其他用户。这两个特性意味着：使用右值引用的代码可以自由地接管所引用的对象的资源。

类似任何引用，一个右值引用也不过是某个对象的另一个名字而已。

右值引用只能绑定到右值，我们通过 `&&` 而不是 `&` 来获得右值引用。

```cpp
int i = 42;
// 正确，r 引用 i
int& r = i;
// 错误，不能将一个右值引用绑定到一个左值上
// int&& rr = i;
// 错误，i*42 是一个右值
// int& r2 = i * 42;
// 正确，我们可以将一个const的引用绑定到一个右值上
const int& r3 = i * 42;
// 正确，将rr2绑定到乘法结果上
int&& rr2 = i * 42;
```

**变量是左值。变量可以看作只有一个运算对象而没有运算符的表达式，虽然我们很少这样看待变量。类似其他任何表达式，变量表达式也有左值/右值属性。变量表达式都是左值。带来的结果就是，我们不能将一个右值引用绑定到一个右值引用类型的变量上**，这有些令人惊讶：

```cpp
// 正确：字面常量是右值
int&& rr1 = 42;

// 错误：表达式rr1是左值
int&& rr2 = rr1;
// 正确：
int&& rr3 = std::move(rr1);
```

其实有了右值表示临时对象这一观察结果，变量是左值这一特性并不令人惊讶。毕竟，变量是持久的，直至离开作用域时才被销毁。

变量是左值，因此我们不能将一个右值引用直接绑定到一个变量上，即使这个变量是右值引用类型也不行。

**编译器将命名的右值引用视为左值，将未命名的右值引用视为右值。** 当我们编写一个将右值引用作为其参数的函数时，该参数在函数体中被视为左值。

```cpp
// vs 是左值，即使 vs 是右值引用类型
void f(vector<string>&& vs)
{
	vector<string> vs2(std::move(vs));	// call move constructor
	//...
}
```

右值引用可用于为临时对象[延长生存期](https://zh.cppreference.com/w/cpp/language/reference_initialization#.E4.B8.B4.E6.97.B6.E9.87.8F.E7.94.9F.E5.AD.98.E6.9C.9F)（注意，const 的左值引用也能延长临时对象生存期，但这些对象无法因此被修改）。临时对象的生命周期由其用法决定。一般情况下，临时对象的生命周期与它所处的完整表达式（full expression）一致；如果临时对象被绑定到一个引用上，则临时对象的生命周期就被延长以匹配引用的生命周期，即临时对象的生命周期就是引用的生命周期。需要说明的是，引用只是有限程度上地延长临时对象生存期，因为通常情况下，临时对象也是自动对象，当离开作用域范围时将被销毁。所以，函数不要 return **局部临时对象**的引用或指针，因为当函数结束时局部临时对象占用的空间也就随之释放掉了，return 的局部临时对象的引用或指针将指向不再有效的内存区域。

## 标准库 move 函数

虽然不能将一个右值引用直接绑定到一个左值上，但我们可以显式地将一个左值转换为对应的右值引用类型。我们还可以通过调用一个名为 `std::move` 的新标准库函数来获得绑定到左值上的右值引用，此函数定义在头文件utility中。

```cpp
int&& rr3 = std::move(rr1);
```

我们必须认识到，调用std::move就意味着承诺：除了对**移后源对象**赋予新值或销毁它之外，我们将不再使用它。在调用std::move之后，我们不能对移后源对象的值做任何假设。



## References

- [值类别 - cppreference.com](https://zh.cppreference.com/w/cpp/language/value_category) 
- [Value categories - cppreference.com](https://en.cppreference.com/w/cpp/language/value_category)
- [Value Categories: Lvalues and Rvalues (C++) | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/lvalues-and-rvalues-visual-cpp?view=msvc-160)
- [Rvalue Reference Declarator: && | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-160)
- [How to: Define move constructors and move assignment operators (C++) | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-160)
- [Temporary object lifetime - cppreference.com](https://en.cppreference.com/w/cpp/language/lifetime#Temporary_object_lifetime)
- [临时延长生存期 - cppreference.com](https://zh.cppreference.com/w/cpp/language/reference_initialization#.E4.B8.B4.E6.97.B6.E9.87.8F.E7.94.9F.E5.AD.98.E6.9C.9F)

