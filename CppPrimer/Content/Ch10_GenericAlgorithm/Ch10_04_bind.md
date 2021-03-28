# ***bind***

## bind 介绍

标准库函数`std::bind`，它定义在头文件`functional`中。可以将`bind`函数看作一个通用的函数适配器，它接受一个可调用对象，生成一个新的可调用对象来“适应”原对象的参数列表。 

调用`bind`的一般形式为： 

```c++
auto newCallable = bind(callable, arg_list);
```

其中，newCallable本身是一个可调用对象，arg_list是一个逗号分隔的参数列表，对应给定的callable的参数。即，当我们调用newCallable时，newCallable会调用callable，并传递给它arg_list中的参数。 

arg_list中的参数可能包含形如`_n`的名字，其中n是一个整数。这些参数是“占位符”，表示newCallable的参数，它们占据了传递给newCallable的参数的“位置”。数值n表示生成的可调用对象中参数的位置：`_1`为newCallable的第一个参数，`_2`为第二个参数，依此类推。

名字`_n`都定义在一个名为`placeholders`的命名空间中，而这个命名空间本身定义在`std`命名空间中。为了使用这些名字，两个命名空间都要写上：

```c++
using namespace std::placeholders;
```

这样便使得由`placeholders`定义的所有名字都可用。与`bind`函数一样，`placeholders`命名空间也定义在`functional`头文件中。

我们也可以用`bind`重排参数顺序。

默认情况下，`bind`的那些不是占位符的参数被拷贝到`bind`返回的可调用对象中。但是，与`lambda`类似，有时对有些绑定的参数我们希望以引用方式传递，或是要绑定参数的类型无法拷贝。

