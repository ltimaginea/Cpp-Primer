#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <functional>

using namespace std;

bool CheckSize(const string& str, string::size_type size)
{
    if (str.size() >= size)
        return true;
    else
        return false;
}

ostream& Print(ostream& os, const string& str, char ch)
{
    return os << str << ch;
}

// words按字典序排序，并去重
void ElimDups(vector<string>& words)
{
    sort(words.begin(), words.end());
    auto end_unique = unique(words.begin(), words.end());
    words.erase(end_unique, words.end());
}

// words按长度排序，长度相同的单词维持字典序。计算大于等于指定长度的元素数目并打印对应单词
void Biggies(vector<string>& words, string::size_type size)
{
    ElimDups(words);
    stable_sort(words.begin(), words.end(), [](const string& a, const string& b) {return a.size() < b.size(); });
    
    using namespace std::placeholders;
    auto it = find_if(words.begin(), words.end(), bind(CheckSize, _1, size));
    auto cnt = words.end() - it;
    for_each(it, words.end(), bind(Print, ref(cout), _1, ' '));
}

int main()
{
    vector<string> words = { "the","quick","red","fox","jumps","over","the","slow","red","turtle" };
    Biggies(words, 4);
    return 0;
}

// tips: 
//   1. 实参 形参 值传递 引用传递 ???