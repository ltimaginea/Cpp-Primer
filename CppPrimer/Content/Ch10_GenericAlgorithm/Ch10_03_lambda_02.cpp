#include <iostream>

using namespace std;

int main()
{
    size_t vl = 42;
    auto f1 = [vl]()mutable { return ++vl; };
    auto f2 = [&vl] { return ++vl; };   // vl 是一个非const变量的引用，因此可以通过f2中的引用来改变它
    vl = 0;
    cout << f1() << endl;               // 43
    cout << f2() << endl;               // 1

    int i = 1, j = 2;
    auto f3 = [&i, j]()mutable { ++i; ++j; };

    return 0;
}

// tips: 
//   1. 默认情况下，对于一个值被拷贝的变量，lambda不会改变其值。如果我们希望能改变一个被捕获的变量的值，就必须在参数列表尾加上关键字mutable。因此，可变lambda能省略参数列表。
//   2. 一个引用捕获的变量是否可以修改依赖于此引用指向的是一个const类型还是一个非const类型。