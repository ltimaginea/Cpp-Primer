# Access Control and Inheritance 2

## Access to virtual functions

See [Access specifiers - cppreference.com](https://en.cppreference.com/w/cpp/language/access) :Access rules for the names of [virtual functions](https://en.cppreference.com/w/cpp/language/virtual) are checked at the call point using the type of the expression used to  denote the object for which the member function is called. The access of the final overrider is ignored.

一般情况下，基类与派生类的同名同参数表的虚函数的访问范围说明符应保持一致。

示例程序见： [Ch15_05_access_to_virtual_functions.cpp](./Ch15_05_access_to_virtual_functions.cpp)

## Friend with Inheritance

类的友元能够访问类对象的所有成员。

就像友元关系不能传递一样，友元关系同样也不能继承。基类的友元在访问派生类对象的成员时不具有特殊性，类似的，派生类的友元在访问基类对象的成员时也不具有特殊性。基类的友元能够访问基类对象的所有成员，这种可访问性包括了基类对象内嵌在其派生类对象中的情况（经过测试发现仅派生类对象是公有继承基类时才正确），见示例程序 [Ch15_05_friend_inheritance.cpp](./Ch15_05_friend_inheritance.cpp) 中的 `int f3(Derived d) { return d.state_; }` 。

当一个类将另一个类声明为友元时，这种友元关系只对做出声明的类有效。对于原来那个类来说，其友元的基类或者派生类不具有特殊的访问能力。

示例程序见： [Ch15_05_friend_inheritance.cpp](./Ch15_05_friend_inheritance.cpp)

## 改变个别成员的可访问性



## 派生类向基类转换的可访问性

派生类向基类的转换是否可访问由使用该转换的代码决定，同时派生类的派生访问说明符也会有影响。假定D继承自B：

- 只有当D公有地继承B时，用户代码才能使用派生类向基类的转换；如果D继承B的方式是受保护的或者私有的，则用户代码不能使用该转换。
- 不论D以什么方式继承B，D的成员函数和友元都能使用派生类向基类的转换；派生类向其直接基类的类型转换对于派生类的成员和友元来说永远是可访问的。
- 如果D继承B的方式是公有的或者受保护的，则D的派生类的成员和友元可以使用D向B的类型转换；反之，如果D继承B的方式是私有的，则不能使用。 

对于代码中的某个给定节点来说，如果基类的公有成员是可访问的，则派生类向基类的类型转换也是可访问的；反之则不行。

## References

- [Access specifiers - cppreference.com](https://en.cppreference.com/w/cpp/language/access)
- [Member Access Control (C++) | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/cpp/member-access-control-cpp?view=msvc-170#access-to-virtual-functions)
- [虚函数的访问权限_ltimaginea的博客-CSDN博客](https://blog.csdn.net/sinat_43125576/article/details/110359051)
- [C.128: Virtual functions should specify exactly one of virtual, override, or final (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override)
- [Ch15_05_AccessControl_Inheritance.md](./Ch15_05_AccessControl_Inheritance.md)



