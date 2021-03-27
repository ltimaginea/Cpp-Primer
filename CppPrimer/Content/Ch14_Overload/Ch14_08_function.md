# ***function***

## 可调用对象

C++语言中有几种可调用的对象：函数、函数指针、lambda表达式、bind创建的对象以及重载了函数调用运算符的类。

和其他对象一样，可调用的对象也有类型。例如，每个lambda有它自己唯一的（匿名）类类型；函数及函数指针的类型则由其返回值类型和实参类型决定，等等。

**然而，两个不同类型的可调用对象却可能共享同一种调用形式（call signature）**。调用形式指明了调用返回的类型以及传递给调用的实参类型。一种调用形式对应一个函数类型，例如：

```c++
int(int, int)
```

是一个函数类型，它接受两个int、返回一个int。

## function 介绍

类模板 `std::function` 是通用多态函数包装器。 `std::function` 的实例能存储、复制及调用任何[*可调用* *(Callable)* ](https://zh.cppreference.com/w/cpp/named_req/Callable)*目标*——函数、 [lambda 表达式](https://zh.cppreference.com/w/cpp/language/lambda)、 [bind 表达式](https://zh.cppreference.com/w/cpp/utility/functional/bind)或其他函数对象，还有指向成员函数指针和指向数据成员指针。

``std::function``定义在functional头文件中。``std::function``是一个类模板，和我们使用过的其他模板一样，当创建一个具体的function类型时我们必须提供额外的信息。所谓额外的信息是指该``std::function``类型能够表示的对象的调用形式。参考其他模板，我们在一对尖括号内指定类型： 

```c++
function<int(int, int)>
```

在这里我们声明了一个``std::function``类型，它可以表示接受两个int、返回一个int的可调用对象。

## function 与重载函数

我们不能直接将重载函数的名字存入 ``std::function`` 类型的对象中。

**解决二义性的途径: 存储函数指针或强制转型到适当的函数指针型别，也可以借助lambda来消除二义性**。

