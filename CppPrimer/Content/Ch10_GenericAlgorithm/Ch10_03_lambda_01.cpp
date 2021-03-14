#include <iostream>
#include <functional>

using namespace std;

function<int(int)> make_offseter(int offset)
{
    return [=](int j) { return offset + j; };
}

int main()
{
    // 显式捕获
    size_t vl = 42;
    auto f1 = [vl] { return vl; };      // 值捕获。值捕获的变量的值是在lambda创建时拷贝，而不是调用时拷贝。
    auto f2 = [&vl] { return vl; };     // 引用捕获
    vl = 0;
    cout << f1() << endl;               // 42
    cout << f2() << endl;               // 0

    // 隐式捕获
    int offset = 1234, j = 5;
    function<int(int)> f3 = make_offseter(offset);
    function<int(int)> f4 = [&](int j) { return offset + j; };
    offset = 6789;
    cout << f3(j) << endl;              // 1239
    cout << f4(j) << endl;              // 6794

    // 混合捕获
    int i = 55;
    function<int()> f5 = [=, &i, &j] { return offset + i + j; };
    function<int()> f6 = [&, i, j] { return offset + i + j; };
    function<int()> f7 = [&offset, i, &j] { return offset + i + j; };
    offset = 135; i = 66; j = 6;
    cout << f5() << endl;               // 6861
    cout << f6() << endl;               // 195
    cout << f7() << endl;               // 196

    return 0;
}

// tips: 
//   1. 闭包是lambda式创建的运行期对象，根据不同的捕获模式，闭包会持有数据的副本或引用，闭包的类型不能被指名，但可用 auto 提及。
//   2. 值捕获的变量的值是在lambda创建时拷贝，而不是调用时拷贝。