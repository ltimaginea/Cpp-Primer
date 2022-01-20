# The rule of three/five/zero

如果类对某种资源进行管理，而资源句柄是非类类型的对象（裸指针、POSIX 文件描述符等），则几乎肯定需要用户定义拷贝控制成员。若用户不定义它们，则编译器会隐式定义。但这些隐式定义的成员函数通常都不正确，其析构函数不做任何事，而复制构造函数、复制赋值运算符则进行“浅复制”（复制句柄的值，而不复制底层资源）。

**所有五个拷贝控制成员应该被看作一个整体，简单来说，要么全部都定义，要么全部都不定义，仅定义一部分是最糟糕的做法（因为拷贝控制成员之间存在着复杂的相互影响）。一般来说，如果一个类定义了任何一个拷贝操作，它就应该定义所有五个操作**。

## The rule of three/five/zero from CppPrimer5e

如果一个类需要自定义析构函数，几乎可以肯定它也需要自定义拷贝赋值运算符和拷贝构造函数。

如果一个类需要一个拷贝构造函数，几乎可以肯定它也需要一个拷贝赋值运算符。反之亦然——如果一个类需要一个拷贝赋值运算符，几乎可以肯定它也需要一个拷贝构造函数。然而，无论是需要拷贝构造函数还是需要拷贝赋值运算符都不必然意味着也需要析构函数。

所有五个拷贝控制成员应该看作一个整体：一般来说，如果一个类定义了任何一个拷贝操作，它就应该定义所有五个操作。如前所述，某些类必须定义拷贝构造函数、拷贝赋值运算符和析构函数才能正确工作。这些类通常拥有一个资源，而拷贝成员必须拷贝此资源。一般来说，拷贝一个资源会导致一些额外开销。在这种拷贝并非必要的情况下，定义了移动构造函数和移动赋值运算符的类就可以避免此问题。

## The rule of three/five/zero from CppReferenceChinese

### 三之法则

若某个类需要用户定义的[析构函数](https://zh.cppreference.com/w/cpp/language/destructor)、用户定义的[复制构造函数](https://zh.cppreference.com/w/cpp/language/copy_constructor)或用户定义的[复制赋值运算符](https://zh.cppreference.com/w/cpp/language/copy_assignment)，则它几乎肯定三者全部都需要。

因为 C++ 在各种场合（按值传递/返回、操纵容器等）对对象进行复制和复制赋值，若可访问，这些特殊成员函数就会被调用，而且若用户不定义他们，则编译器会隐式定义。

如果类对某种资源进行管理，而资源句柄是非类类型的对象（裸指针、POSIX 文件描述符等），则这些隐式定义的成员函数通常都不正确，其析构函数不做任何事，而复制构造函数/复制赋值运算符则进行“浅复制”（复制句柄的值，而不复制底层资源）。

通过可复制句柄来管理不可复制资源的类，可能必须将其复制赋值和复制构造函数声明为私有的并且不提供其定义，或将它们定义为 `=delete` 。这是三之法则的另一种应用：删除其中之一而遗留其他被隐式定义很可能会导致错误。

### 五之法则

因为用户定义析构函数、复制构造函数或复制赋值运算符的存在阻止[移动构造函数](https://zh.cppreference.com/w/cpp/language/move_constructor)和[移动赋值运算符](https://zh.cppreference.com/w/cpp/language/move_assignment)的隐式定义，所以任何想要移动语义的类必须声明全部五个特殊成员函数。

与三之法则不同的是，不提供移动构造函数和移动赋值运算符通常不是错误，但会导致失去优化机会。

### 零之法则

有自定义析构函数、复制/移动构造函数或复制/移动赋值运算符的类应该专门处理所有权（这遵循[单一责任原则](https://en.wikipedia.org/wiki/Single_responsibility_principle)）。其他类都不应该拥有自定义的析构函数、复制/移动构造函数或复制/移动赋值运算符。

这条法则也在 C++ 核心指南（C++ Core Guidelines）中出现，为 [C.20：一旦可以避免定义默认操作就应当施行](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero)。

```cpp
class rule_of_zero
{
    std::string cppstring;
 public:
    rule_of_zero(const std::string& arg) : cppstring(arg) {}
};
```

当有意将某个基类用于多态用途时，可能必须将其析构函数声明为公开的虚函数。由于这会阻拦隐式移动（并弃用隐式复制）的生成，因而必须将各特殊成员函数声明为 `=default` 。

```cpp
class base_of_five_defaults
{
 public:
    base_of_five_defaults(const base_of_five_defaults&) = default;
    base_of_five_defaults(base_of_five_defaults&&) = default;
    base_of_five_defaults& operator=(const base_of_five_defaults&) = default;
    base_of_five_defaults& operator=(base_of_five_defaults&&) = default;
    virtual ~base_of_five_defaults() = default;
};
```

然而这使得类有可能被切片，这是多态类经常把复制和移动操作定义为 `=delete` 或者 `protected` 并添加一个 virtual `clone` 函数的原因。如果类没有数据，把复制和移动操作定义为 `=delete` ，否则，定义为 `protected` （见 [C.67](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-copy-virtual) , [C.130](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-copy) ）。这带来了下面的五之法则的通用说法：

[C.21：若有任何默认操作被定义或 =delete，则应当对它们全部进行定义或 =delete](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-five)

## The rule of three/five/zero from CppReferenceEnglish

### Rule of three

If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

Because C++ copies and copy-assigns objects of user-defined types in various situations (passing/returning by value, manipulating a container, etc), these special member functions will be called, if accessible, and if they are not user-defined, they are implicitly-defined by the compiler.

The implicitly-defined special member functions are typically incorrect if the class manages a resource whose handle is an object of non-class type (raw pointer, POSIX file descriptor, etc), whose destructor does nothing and copy constructor/assignment operator performs a "shallow copy" (copy the value of the handle, without duplicating the underlying resource).

Classes that manage non-copyable resources through copyable handles may have to declare copy assignment and copy constructor private and not provide their definitions or define them as deleted. This is another application of the rule of three: deleting one and leaving the other to be implicitly-defined will most likely result in errors.

### Rule of five

Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) and the [move assignment operator](https://en.cppreference.com/w/cpp/language/move_operator), any class for which move semantics are desirable, has to declare all five special member functions.

Unlike Rule of Three, failing to provide move constructor and move assignment is usually not an error, but a missed optimization opportunity.

### Rule of zero

Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership (which follows from the [Single Responsibility Principle](https://en.wikipedia.org/wiki/Single_responsibility_principle)). Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators.

This rule also appears in the C++ Core Guidelines as [C.20: If you can avoid defining default operations, do](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero).

```cpp
class rule_of_zero
{
    std::string cppstring;
 public:
    rule_of_zero(const std::string& arg) : cppstring(arg) {}
};
```

When a base class is intended for polymorphic use, its destructor may have to be declared public and virtual. This blocks implicit moves (and deprecates implicit copies), and so the special member functions have to be declared as defaulted.

```cpp
class base_of_five_defaults
{
 public:
    base_of_five_defaults(const base_of_five_defaults&) = default;
    base_of_five_defaults(base_of_five_defaults&&) = default;
    base_of_five_defaults& operator=(const base_of_five_defaults&) = default;
    base_of_five_defaults& operator=(base_of_five_defaults&&) = default;
    virtual ~base_of_five_defaults() = default;
};
```

However, this makes the class prone to slicing, which is why polymorphic classes often make the copy and move operations  `=delete` or `protected` , and add a virtual `clone` function. If the class has no data, `=delete` the copy and move operations, otherwise, make them  `protected`  (see [C.67](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-copy-virtual) , [C.130](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-copy) in C++ Core Guidelines), which leads to the following generic wording for the Rule of Five:

[C.21: If you define or =delete any default operation, define or =delete them all](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-five) 



## References

- [The rule of three/five/zero - cppreference.com](https://en.cppreference.com/w/cpp/language/rule_of_three)
- [三/五/零之法则 - cppreference.com](https://zh.cppreference.com/w/cpp/language/rule_of_three)
- [C.20: If you can avoid defining default operations, do (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c20-if-you-can-avoid-defining-default-operations-do)
- [C.21: If you define or =delete any copy, move, or destructor function, define or =delete them all (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c21-if-you-define-or-delete-any-copy-move-or-destructor-function-define-or-delete-them-all) 
- [C.67: A polymorphic class should suppress public copy and move (CppCoreGuidelines)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-copy-virtual) 
- [C.130: For making deep copies of polymorphic classes prefer a virtual clone function instead of public copy construction and assignment (CppCoreGuidelines)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-copy) 
- [Copyable_Movable_Types (Google C++ Style Guide)](https://google.github.io/styleguide/cppguide.html#Copyable_Movable_Types)

