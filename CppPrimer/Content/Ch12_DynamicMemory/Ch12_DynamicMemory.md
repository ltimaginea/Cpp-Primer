# ***Dynamic Memory***

`全局对象` 在程序启动时分配，在程序结束时销毁。对于 `局部自动对象` ，当我们进入其定义所在的程序块时被创建，在离开块时销毁。 `static对象` 在第一次使用前分配，在程序结束时销毁。 除了自动和static对象外，C++还支持 `动态分配对象` 。动态分配的对象的生存期与它们在哪里创建是无关的，只有当显式地被释放时，这些对象才会销毁。

`静态内存` 用来保存局部static对象、类static数据成员以及定义在任何函数之外的变量。 `栈内存` 用来保存定义在函数内的非static对象。分配在静态或栈内存中的对象由编译器自动创建和销毁。对于栈对象，仅在其定义的程序块运行时才存在；static对象在使用之前分配，在程序结束时销毁。 除了静态内存和栈内存，每个程序还拥有一个 `内存池` 。这部分内存被称作 `自由空间` （free store）或 `堆` （heap）。程序用堆来存储动态分配（dynamically allocate）的对象——即，那些在程序运行时分配的对象。动态对象的生存期由程序来控制，也就是说，当动态对象不再使用时，我们的代码必须显式地销毁它们。

在C++中，动态内存的管理是通过一对运算符来完成的： `new` ，在动态内存中为对象分配内存空间并返回一个指向该对象的指针，我们可以选择对对象进行初始化； `delete` ，接受一个动态对象的指针，销毁该对象，并释放与之关联的内存。 动态内存的使用很容易出问题，因为确保在正确的时间释放内存是极其困难的。有时我们会忘记释放内存，在这种情况下就会产生内存泄漏；有时在尚有指针引用内存的情况下我们就释放了它，在这种情况下就会产生引用非法内存的指针。

为了更容易（同时也更安全）地使用动态内存，新的标准库提供了两种 `智能指针` （smart pointer）类型来管理动态对象。智能指针的行为类似常规指针，重要的区别是它负责自动释放所指向的对象。新标准库提供的这两种智能指针的区别在于管理底层指针的方式： `shared_ptr` 允许多个指针指向同一个对象； `unique_ptr` 则“独占”所指向的对象。标准库还定义了一个名为 `weak_ptr` 的伴随类，它是一种弱引用，指向shared_ptr所管理的对象。这三种类型都定义在memory头文件中。

程序使用动态内存出于以下三种原因之一：

- 程序不知道自己需要使用多少对象。
- 程序不知道所需对象的准确类型。
- 程序需要在多个对象间共享数据。
