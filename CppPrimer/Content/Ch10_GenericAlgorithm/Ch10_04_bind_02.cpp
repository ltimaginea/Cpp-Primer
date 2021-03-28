#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <functional>

using namespace std;

bool Comp(const string& a, const string& b)
{
	return a.size() < b.size();
}

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

	using namespace std::placeholders;

	stable_sort(words.begin(), words.end(), bind(Comp, _1, _2));
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
//   1. 绑定对象的所以实参（占位符参数）都是按引用传递的，因为此种对象的函数调用运算符利用了完美转发。
//   2. 但是，bind 的那些不是占位符的参数默认是被拷贝到 bind 返回的可调用对象中。与 lambda 类似，有时对有些绑定的参数我们希望以引用方式传递，或是要绑定参数的类型无法拷贝。
//	这时，我们可以通过实施 std::ref 或者 std::cref 的方法达成按引用传递的效果。与 bind 一样，函数 ref 和 cref 也定义在头文件 functional 中。