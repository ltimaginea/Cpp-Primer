#include <iostream>
#include <future>
#include <thread>

using namespace std;

/*
* C++14还添加了广义捕获的概念，因此可以捕获表达式的结果，而不是对局部变量的直接拷贝或引用。最常见的方法是通过移动只移动的类型来捕获类型，而不是通过引用来捕获。
* 这里，promise通过p=std::move(p)捕获移到Lambda中，因此可以安全地分离线程，从而不用担心对局部变量的悬空引用。构建Lambda之后，p处于转移过来的状态，这就是为什么需要提前获得future的原因。
*/
future<int> spawn_async_task()
{
    std::promise<int> p;
    auto f = p.get_future();
    std::thread t([p = std::move(p)](){ p.set_value(find_the_answer()); });
    t.detach();
    return f;
}

int main()
{

    /*
    * C++14后，Lambda表达式可以是真正通用Lamdba了，参数类型被声明为auto而不是指定类型。这种情况下，函数调用运算也是一个模板。当调用Lambda时，参数的类型可从提供的参数中推导出来。
    */
    auto f = [](auto x) {cout << x << endl; };
    f(233);
    f("C++14 lambda");
    
    return 0;
}