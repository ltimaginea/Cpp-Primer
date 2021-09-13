# 对象和值

我们可以分配并使用没有名字的对象（比如用new创建的对象），也能为某些看起来不太寻常的表达式赋值（如，`*p[a+10]=7`）。因此，我们需要用一个名字来表示“内存中的某个东西”。这就是对象一词最简单和最基本的含义。换句话说，对象（object）是指一块连续存储区域，**左值**（lvalue）是指向对象的一条表达式。“左值”的字面意思是“能用在赋值运算符左侧的东西”，但其实不是所有左值都能用在赋值运算符的左侧，左值也有可能指示某个常量。未被声明成const的左值称为可修改的左值（modifiable  lvalue）。此处我们提到的对象的最简单和最基本的含义不应该与类的对象或多态类型的对象混淆。 

## 左值和右值

为了补充和完善左值的含义，我们相应地定义了**右值**（rvalue）。简单来说，右值是指“不能作为左值的值”，比如像函数返回值一样的临时值。

如果你希望技术性更强一些（比如你想读一下ISO  C++标准），那就需要更新看待左值和右值的视角了。当考虑对象的寻址、拷贝、移动等操作时，有两种属性非常关键。

- **有身份**（Has  identity）：在程序中有对象的名字，或指向该对象的指针，或该对象的引用，这样我们就能判断两个对象是否相等或者对象的值是否发生了改变。

- **可移动**（Is  movable）：能把对象的内容移动出来（比如，我们能把它的值移动到其他某处，剩下的对象处于合法但未指定的状态，与拷贝是有差别的）。

在上述两个属性的四种组合形式中，有三种需要用C++语言规则精确地描述（既没有身份又不能移动的对象不重要）。我们“**用m表示可移动**”，且“**用i表示有身份**”，从而把表达式的分类表示成下图所示的形式：

![](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Images/Chapter13/Ch13_06_ValueCategory0.png)

从图中可知，一个经典的左值有身份但不能移动（因为我们可能会在移动后仍然使用它），而一个经典的右值是允许执行移出操作的对象。其他一些有关的术语还包括**纯右值**（prvalue，“纯 (pure)”的右值）、**泛左值**（glvalue，“泛化 (generalized)”的左值）和**特别值**（xvalue，即 extraordinary value；又称为 专家值，expert only value；又称为 亡值，“将亡 (expiring)”的值）。例如：

```cpp
void f(vector<string>&& vs)
{
	vector<string> vs2(std::move(vs));
	//...
}
```

此处， std::move(vs) 是一个特别值（xvalue）。它明显有身份（我们能像vs一样引用它），并且我们显式地给予了将其值移出的许可，方式是调用 std::move(vs) 。

在实际编程过程中，考虑左值和右值就足够了。一条表达式要么是左值，要么是右值，不可能两者都是。当然，准确地“二分”应该是 泛左值（glvalue）和 纯右值（prvalue）。



> ## *References*
>
> ### [Value categories - cppreference.com](https://en.cppreference.com/w/cpp/language/value_category)
>
> ### [Value Categories: Lvalues and Rvalues (C++) | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/lvalues-and-rvalues-visual-cpp?view=msvc-160)
>
> The following diagram illustrates the relationships between the categories:
>
> ![](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Images/Chapter13/Ch13_06_ValueCategory1.png)
>
> An **lvalue** has an address that your program can access. Examples of lvalue expressions include variable names, including **`const`** variables, array elements, function calls that return an lvalue reference, bit-fields, unions, and class members.
>
> A **prvalue** expression has no address that is accessible by your program. Examples of prvalue expressions include literals, function calls that return a non-reference type, and temporary objects that are created during expression evaluation but accessible only by the compiler.
>
> An **xvalue** expression has an address that no longer accessible by your program but can be used to initialize an rvalue reference, which provides access to the expression. Examples include function calls that return an rvalue reference (e.g., `std::move()` ), and the array subscript, member and pointer to member expressions where the array or object is an rvalue reference.