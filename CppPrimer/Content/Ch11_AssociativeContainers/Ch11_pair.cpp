#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

int main()
{
	vector<pair<string, int>> vec;
	string str;
	int i;

	while (cin >> str >> i)
	{
		vec.push_back(pair<string, int>(str, i));
		vec.push_back(make_pair(str, i));
		vec.push_back({ str,i });		// 用花括号包围的初始化器来返回pair类型对象
		vec.emplace_back(str, i);
		vec.push_back(vector<pair<string, int>>::value_type(str, i));
	}

	for (const auto& p : vec)
		cout << p.first << ":" << p.second << endl;

	return 0;
}

// tips: 
//   1. pair的数据成员first和second是public的。