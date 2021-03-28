# ***bind***

标准库函数 `std::bind` ，它定义在头文件 `functional` 中。可以将 `bind` 函数看作一个通用的函数适配器，它接受一个可调用对象，生成一个新的可调用对象来“适应”原对象的参数列表。 `bind` 生成的函数对象称为**绑定对象**。

调用 `bind` 的一般形式为： 

```c++
auto newCallable = bind(callable, arg_list);
```

其中，newCallable本身是一个可调用对象，arg_list是一个逗号分隔的参数列表，对应给定的callable的参数。即，当我们调用newCallable时，newCallable会调用callable，并传递给它arg_list中的参数。 

arg_list中的参数可能包含形如 `_n` 的名字，其中n是一个整数。这些参数是“**占位符**”，表示newCallable的参数，它们占据了传递给newCallable的参数的“位置”。数值n表示生成的可调用对象中参数的位置： `_1` 为newCallable的第一个参数， `_2` 为第二个参数，依此类推。

名字 `_n` 都定义在一个名为 `placeholders` 的命名空间中，而这个命名空间本身定义在 `std` 命名空间中。为了使用这些名字，两个命名空间都要写上：

```c++
using namespace std::placeholders;
```

这样便使得由 `placeholders` 定义的所有名字都可用。与 `bind` 函数一样， `placeholders` 命名空间也定义在 `functional` 头文件中。

我们也可以用 `bind` 重排参数顺序。

`bind` 无法直接使用重载函数，需要强制转型到适当的函数指针型别。

绑定对象的所以实参（占位符参数）都是按**引用**传递的，因为此种对象的函数调用运算符利用了**完美转发**。

但是， `bind` 的那些不是占位符的参数默认是被**拷贝**到 `bind` 返回的可调用对象中。与 `lambda` 类似，有时对有些绑定的参数我们希望以引用方式传递，或是要绑定参数的类型无法拷贝。这时，我们可以通过实施 `std::ref` 或者 `std::cref` 的方法达成按引用传递的效果。与 `bind` 一样，函数 `ref` 和 `cref` 也定义在头文件 `functional` 中。

## ***Tips***

- 优先选用 `lambda` ，而非 `bind` ，因为 `lambda` 比起使用 `bind` 而言，可读性更好、表达力更强、可能运行效率也更高。



