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