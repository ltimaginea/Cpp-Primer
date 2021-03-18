#include <iostream>
#include <string>

#include <functional>

using namespace std;

void Goodbye(const string& name)
{
    cout << name << ",goodbye!" << endl;
}

void Goodbye(const string& name1, const string& name2)
{
    cout << name1 << "," << name2 << ",goodbye!" << endl;
}

int main()
{
    using namespace std::placeholders;

    using Goodbye1ParamType = void(*)(const string& name);
    using Goodbye2ParamType = void(*)(const string& name1, const string& name2);
    
    auto f1 = bind(static_cast<Goodbye1ParamType>(Goodbye), _1);
    auto f2 = bind(static_cast<Goodbye2ParamType>(Goodbye), _1, _2);
    
    f1("Scott Meyers");
    f2("Scott Meyers", "Herb Sutter");
    
    return 0;
}

// tips: 
//   1. bind 无法直接使用重载函数，需要强制转型到适当的函数指针型别。