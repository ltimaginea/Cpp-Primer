#include <iostream>
#include <string>
#include <map>

#include <functional>

using namespace std;
using namespace std::placeholders;

int add(int i, int j)
{
    return i + j;
}

struct divide {
    int operator()(int denominator, int divisor) {
        return denominator / divisor;
    }
};

auto l_mod = [](int i, int j) { return i % j; };

int mod(int i, int j)
{
    return i % j;
}

int main()
{
    map<string, function<int(int, int)>> binops = {
        {"+",add},                                      // 函数指针
        {"-",std::minus<int>()},                        // 标准库函数对象
        {"/",divide()},                                 // 函数对象类的对象（其类构造函数应该会生成一个匿名对象）
        {"*",[](int i,int j) { return i * j; }},        // 匿名 lambda
        //{"%",l_mod}                                   // 命名了的 lambda 对象
        {"%",bind(mod,_1,_2)}                           // bind 表达式
    };

    cout << binops["+"](10, 5) << endl;                 // 调用 add(10,5)
    cout << binops["-"](10, 5) << endl;                 // 使用 minus<int> 对象的调用运算符
    cout << binops["/"](10, 5) << endl;                 // 使用 divide 对象的调用运算符
    cout << binops["*"](10, 5) << endl;                 // 调用 lambda 函数对象
    cout << binops["%"](10, 5) << endl;                 // 调用 bind 表达式

    return 0;
}

// tips: 
//   1. 类模板 std::function 是通用多态函数封装器。 std::function 的实例能存储、复制及调用任何可调用 (Callable) 目标:
//      函数、 lambda 表达式、 bind 表达式或其他函数对象，还有指向成员函数指针和指向数据成员指针。