#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

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
    auto it = find_if(words.begin(), words.end(), [size](const string& str) {return str.size() >= size; });
    auto cnt = words.end() - it;
    for_each(it, words.end(), [](const string& str) {cout << str << " "; });
}

int main()
{
    vector<string> words = { "the","quick","red","fox","jumps","over","the","slow","red","turtle" };
    Biggies(words, 4);
    return 0;
}