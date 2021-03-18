#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

void f(int n1, int n2, int n3, int n4, int n5)
{
    cout << n1 << " " << n2 << " " << n3 << " " << n4 << " " << n5 << endl;
}

int main()
{
    using namespace std::placeholders;
    auto g = bind(f, _5, _4, _3, _2, _1);
    g(100, 200, 300, 400, 500);         // g(_1, _2, _3, _4, _5)映射为f(_5, _4, _3, _2, _1),即 f(500,400,300,200,100)
    return 0;
}

// tips: 
//   1. 如果忽略返回类型，lambda根据函数体中的代码推断出返回类型。
//      如果函数体只是一个return语句，则返回类型从返回的表达式的类型推断而来。否则，返回类型为void，即如果函数体包含单一return语句之外的任何语句，则编译器推断此lambda返回void。这时就需要显式的指定返回类型。
//      返回类型可以使用跟在参数列表后面的箭头(->)进行设置。
//      如果Lambda函数没有任何参数，还需要包含(空)的参数列表，这样做是为了能显式的对返回类型进行指定。如 cond.wait(lk, []()->bool { return data_ready; });