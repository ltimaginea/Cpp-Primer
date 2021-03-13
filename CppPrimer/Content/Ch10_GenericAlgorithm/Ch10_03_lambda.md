# ***lambda 表达式***

## 介绍 lambda

一个lambda表达式表示一个可调用的代码单元。我们可以将其理解为一个未命名的内联函数（**匿名函数**）。与任何函数类似，一个lambda具有一个返回类型、一个参数列表和一个函数体。但与函数不同，lambda可能定义在函数内部。一个lambda表达式具有如下形式

```c++
[capture list] (parameter list) -> return type { function body }
```

其中，capture list（捕获列表）是一个lambda所在函数中定义的局部变量的列表（通常为空）；return type、parameter list和function body与任何普通函数一样，分别表示返回类型、参数列表和函数体。但是，与普通函数不同，lambda必须使用尾置返回来指定返回类型。 

我们可以忽略参数列表和返回类型，但必须永远包含捕获列表和函数体。在lambda中忽略括号和参数列表等价于指定一个空参数列表。如果忽略返回类型，lambda根据函数体中的代码推断出返回类型。如果函数体只是一个return语句，则返回类型从返回的表达式的类型推断而来。否则，返回类型为void。

lambda是函数对象。当我们编写了一个lambda后，编译器将该表达式翻译成一个未命名类的未命名对象（**匿名函数对象**）。在lambda表达式产生的类中含有一个重载的函数调用运算符。当使用auto定义一个用lambda初始化的变量时，即定义了一个从lambda生成的类型的对象。

自C++ 14起，lambda可以拥有默认参数。

```c++
void biggies(vector<string>& words, string::size_type size)
{
    ElimDups(words);
    stable_sort(words.begin(), words.end(), [](const string& a, const string& b) {return a.size() < b.size(); });
    auto it = find_if(words.begin(), words.end(), [size](const string& str) {return str.size() >= size; });
    auto cnt = words.end() - it;
    for_each(it, words.end(), [](const string& str) {cout << str << " "; });
}
```



## 捕获列表

**虽然一个lambda可以出现在一个函数中，使用其局部变量，但它只能使用那些明确指明的变量**。一个lambda通过将局部变量包含在其捕获列表中来指出将会使用这些变量。捕获列表指引lambda在其内部包含访问局部变量所需的信息。

捕获列表只用于局部非static变量，lambda可以直接使用局部static变量和在它所在函数之外声明的名字。

### 值捕获

类似参数传递，变量的捕获方式也可以是值或引用。与传值参数类似，采用值捕获的前提是变量可以拷贝。与参数不同，被捕获的变量的值是在lambda创建时拷贝，而不是调用时拷贝：

### 引用捕获

我们定义lambda时可以采用引用方式捕获变量。例如：

我们也可以从一个函数返回lambda。函数可以直接返回一个可调用对象，或者返回一个类对象，该类含有可调用对象的数据成员。如果函数返回一个lambda，则与函数不能返回一个局部变量的引用类似，此lambda也不能包含引用捕获。

当以引用方式捕获一个变量时，必须保证在lambda执行时变量是存在的。

### 隐式捕获

除了显式列出我们希望使用的来自所在函数的变量之外，还可以让编译器根据lambda体中的代码来推断我们要使用哪些变量。为了指示编译器推断捕获列表，应在捕获列表中写一个&或=。&告诉编译器采用捕获引用方式，=则表示采用值捕获方式。例如，我们可以重写传递给find_if的lambda：

**显式地列出lambda式所依赖的局部变量或形参是更好的软件工程实践**。

如果我们希望对一部分变量采用值捕获，对其他变量采用引用捕获，可以混合使用隐式捕获和显式捕获：

当我们混合使用隐式捕获和显式捕获时，捕获列表中的第一个元素必须是一个&或=。此符号指定了默认捕获方式为引用或值。

当混合使用隐式捕获和显式捕获时，显式捕获的变量必须使用与隐式捕获不同的方式。

![](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Images/Chapter10/Ch10_01_lambdaCaptureList.jpg)

## 可变lambda

默认情况下，对于一个值被拷贝的变量，lambda不会改变其值。如果我们希望能改变一个被捕获的变量的值，就必须在参数列表首加上关键字mutable。因此，可变lambda能省略参数列表：

一个引用捕获的变量是否（如往常一样）可以修改依赖于此引用指向的是一个const类型还是一个非const类型：

## 指定lambda返回类型

默认情况下，如果一个lambda体包含return之外的任何语句，则编译器假定此lambda返回void。


