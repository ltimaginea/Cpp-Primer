# Move Semantics

## 移动构造函数和移动赋值运算符

类似 string 类（及其他标准库类），如果我们自己的类也同时支持移动和拷贝，那么也能从中受益。为了让我们自己的类型支持移动操作（move operation），需要为其定义移动构造函数和移动赋值运算符。这两个成员类似对应的拷贝操作，但它们从给定对象“窃取”资源而不是拷贝资源。

对于内置类型，如 `int` 和 `double*` ，被认为具有移动操作，其实就是简单的拷贝。

### 移动构造函数

如果一个构造函数的第一个参数是自身类类型的右值引用，且任何额外参数都有默认值，则此构造函数是移动构造函数。如果我们的移动构造函数不抛出任何异常，我们就应该将它标记为 noexcept 。

举例： [Ch13_05_StrVec.cpp at main · ltimaginea/Cpp-Primer · GitHub](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_05_StrVec.cpp) 

```cpp
StrVec::StrVec(StrVec&& sv) noexcept :elements_(sv.elements_), first_free_(sv.first_free_), cap_(sv.cap_)
{
	sv.elements_ = nullptr;
	sv.first_free_ = nullptr;
	sv.cap_ = nullptr;
}
```

与拷贝构造函数不同，移动构造函数不分配任何新内存；它接管给定的StrVec中的内存。在接管内存之后，它将给定对象中的指针都置为nullptr。这样就完成了从给定对象的移动操作，此对象将继续存在。最终，移后源对象会被销毁，意味着将在其上运行析构函数。StrVec的析构函数在first_free上调用deallocate。如果我们忘记了改变sv.first_free，则销毁移后源对象就会释放掉我们刚刚移动的内存。

### 移动赋值运算符

移动赋值运算符执行与析构函数和移动构造函数相同的工作。与移动构造函数一样，如果我们的移动赋值运算符不抛出任何异常，我们就应该将它标记为 noexcept 。类似拷贝赋值运算符，移动赋值运算符必须正确处理自赋值。

举例： [Ch13_05_StrVec.cpp at main · ltimaginea/Cpp-Primer · GitHub](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_05_StrVec.cpp) 

```cpp
StrVec& StrVec::operator=(StrVec&& sv) noexcept
{
	// direct test for self-assignment
	if (this != &sv)
	{
		free();
		elements_ = sv.elements_;
		first_free_ = sv.first_free_;
		cap_ = sv.cap_;
		sv.elements_ = nullptr;
		sv.first_free_ = nullptr;
		sv.cap_ = nullptr;
	}
	return *this;
}
```

在此例中，我们直接检查this指针与rhs的地址是否相同。如果相同，右侧和左侧运算对象指向相同的对象，我们不需要做任何事情。否则，我们释放左侧运算对象所使用的内存，并接管给定对象的内存。与移动构造函数一样，我们将rhs中的指针置为nullptr。



一般来说，我们不需要为函数操作定义接受一个 `const T&&` 或是一个（普通的） `T&` 参数的版本。当我们希望从实参“窃取”数据时，通常传递一个右值引用。为了达到这一目的，实参不能是 const 的。类似的，从一个对象进行拷贝的操作不应该改变该对象。因此，通常不需要定义一个接受一个（普通的） T& 参数的版本。

## 移后源对象置为析构安全和有效的状态

从一个对象移动数据并不会销毁此对象，但有时在移动操作完成后，源对象会被销毁。因此，当我们编写一个移动操作时，必须确保移后源对象进入一个析构安全的状态和 [有效的状态](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state) 。

当编写一个移动操作时，我们应该： **(1) 保证将移后源对象（moved-from object）置为析构安全的状态；(2) 保证移后源对象仍然是有效的，一般来说，移后源对象有效就是指可以安全地为移后源对象赋予新值或者可以安全地使用移后源对象而不依赖其当前值**。此外，移动操作对移后源对象中留下的值没有任何要求，因此，我们的程序不应该依赖于移后源对象中的数据。一个比较好的设计是，类的移动操作将移后源对象置于与默认初始化的对象相同的状态。

标准库保证“移后源”（moved-from）对象仍然保持一个析构安全的状态和有效的状态，但我们不能对其值进行任何假设。

## 移动操作、标准库容器和异常

**不抛出异常的移动构造函数和移动赋值运算符应该标记为 `noexcept`** 。

由于移动操作“窃取”资源，它**通常**不分配任何资源。因此，移动操作通常不会抛出任何异常。当编写一个不抛出异常的移动操作时，我们应该将此事通知标准库。一种通知标准库的方法是在我们的构造函数中指明 noexcept ， noexcept 是我们承诺一个函数不抛出异常的一种方法。我们在一个函数的参数列表后指定 noexcept 。在一个构造函数中， noexcept 出现在参数列表和初始化列表开始的冒号之间。

我们必须在类头文件的声明中和定义中（如果定义在类外的话）**都**指定 noexcept 。 

搞清楚为什么需要 noexcept 能帮助我们深入理解标准库是如何与我们自定义的类型交互的。我们需要指出一个移动操作不抛出异常，这是因为两个相互关联的事实：首先，虽然移动操作通常不抛出异常，但抛出异常也是允许的；其次，标准库容器需要对异常发生时其自身的行为提供保障，即保证是异常安全的。

例如，vector保证，如果我们调用push_back时发生异常，vector自身不会发生改变，即保证是异常安全的。对一个vector调用push_back可能要求为vector重新分配内存空间，当重新分配vector的内存完成后，vector会将元素从旧空间移动到新内存中，如果在移动了部分而不是全部元素后抛出了一个异常，就会产生问题：旧空间中的移动源元素已经被改变了，而新空间中未构造的元素可能尚不存在。在此情况下，vector将不能满足自身保持不变的要求。为了避免这种潜在问题，除非vector知道元素类型的移动构造函数不会抛出异常，否则在重新分配内存的过程中，它就必须使用拷贝构造函数而不是移动构造函数。如果希望在vector重新分配内存这类情况下对我们自定义类型的对象进行移动而不是拷贝，就必须显式地告诉标准库我们的移动构造函数可以安全使用。我们通过将移动构造函数（及移动赋值运算符）标记为 noexcept 来做到这一点。

## 合成的移动操作

只有当一个类没有定义任何自己版本的拷贝控制成员，且类的每个非static数据成员都可以移动时，编译器才会为它合成移动构造函数或移动赋值运算符。

与拷贝操作不同，移动操作永远不会隐式定义为删除的函数。但是，如果我们显式地要求编译器生成`=default`的移动操作，且编译器不能移动所有成员，则编译器会将移动操作定义为删除的函数。

移动操作和合成的拷贝控制成员间还有最后一个相互作用关系：一个类是否定义了自己的移动操作对拷贝操作如何合成有影响。如果类定义了一个移动构造函数 和/或 一个移动赋值运算符，则该类的合成拷贝构造函数和拷贝赋值运算符 都 会被定义为删除的。

详细内容参见：  [Ch13_00_CopyControl.md at main · ltimaginea/Cpp-Primer · GitHub](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_00_CopyControl.md#合成的移动操作)

## 移动右值，拷贝左值

如果一个类既有移动构造函数，也有拷贝构造函数，编译器使用普通的函数匹配规则来确定使用哪个构造函数。赋值操作的情况类似。

例如，在我们的 OverloadRule 类中，拷贝构造函数接受一个 `const OverloadRule&` 的引用，因此它可以用于任何可以转换为 OverloadRule 的类型，此版本从其参数拷贝数据。而移动构造函数接受一个 `OverloadRule&&` ，因此它只能用于实参是（非const）右值的情形，此版本会从其参数“窃取”数据。赋值操作的情况类似。

在实参是右值的情形下，copy操作和move操作都是可行的。但调用copy操作需要进行一次到const的转换，而move操作则是精确匹配（也是更好的匹配）的。因此，当我们传递一个可修改的右值时，编译器会选择运行move版本。

```cpp
#include <iostream>
#include <string>
#include <utility>

class OverloadRule
{
public:
	OverloadRule(std::string str = std::string()) :str_(str) {  }
	OverloadRule(const OverloadRule&) = default;
	OverloadRule(OverloadRule&&) = default;
	OverloadRule& operator=(const OverloadRule&) = default;
	OverloadRule& operator=(OverloadRule&&) = default;
	~OverloadRule() = default;
private:
	std::string str_;
};

int main()
{
	OverloadRule or1("courage"), or2("confidence");
	OverloadRule or3(or1);
	OverloadRule or4(std::move(or2));

	OverloadRule or5("spring"), or6("summer"), or7("autumn"), or8("winter");
	or5 = or6;
	or7 = std::move(or8);

	return 0;
}
```



## 如果没有移动操作，右值将被拷贝

如果一个类有一个拷贝构造函数但未定义移动构造函数，会发生什么呢？在此情况下，编译器不会合成移动构造函数，这意味着此类将有拷贝构造函数但不会有移动构造函数。如果一个类没有移动构造函数，函数匹配规则保证该类型的对象会被拷贝，即使我们试图通过调用 `std::move()` 来移动它们时也是如此。赋值操作的情况类似。copy操作是可行的，因为我们可以将一个 `T&&` 转换为一个 `const T&` 。

值得注意的是，用拷贝构造函数代替移动构造函数几乎肯定是安全的（赋值运算符的情况类似）。一般情况下，拷贝构造函数满足对应的移动构造函数的要求：它会拷贝给定对象，并将原对象置于有效状态。实际上，拷贝构造函数甚至都不会改变原对象的值。

```cpp
#include <iostream>
#include <string>
#include <utility>

class OverloadRule
{
public:
	OverloadRule(std::string str = std::string()) :str_(str) {  }
	OverloadRule(const OverloadRule&) = default;
	//OverloadRule(OverloadRule&&) = default;
	OverloadRule& operator=(const OverloadRule&) = default;
	//OverloadRule& operator=(OverloadRule&&) = default;
	~OverloadRule() = default;
private:
	std::string str_;
};

int main()
{
	OverloadRule or1("courage"), or2("confidence");
	OverloadRule or3(or1);
	OverloadRule or4(std::move(or2));

	OverloadRule or5("spring"), or6("summer"), or7("autumn"), or8("winter");
	or5 = or6;
	or7 = std::move(or8);

	return 0;
}
```



## Reference Qualifier

Reference Qualifier 参见：

-  [Cpp-Primer/Ch13_06_ReferenceQualifier.md at main · ltimaginea/Cpp-Primer · GitHub](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_06_ReferenceQualifier.md)
-  [Cpp-Primer/Ch13_06_ReferenceQualifier.cpp at main · ltimaginea/Cpp-Primer · GitHub](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_06_ReferenceQualifier.cpp)



> ## References
>
> [令移后源对象遗留于有效状态](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)
>
> [Ch13_00_Copy_and_Move.md at main · ltimaginea/Cpp-Primer · GitHub](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Content/Ch13_CopyControl/Ch13_00_Copy_and_Move.md)
>
> 
