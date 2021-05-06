# ***pair***

`std::pair` 是类模板， `pair` 是 [std::tuple](https://zh.cppreference.com/w/cpp/utility/tuple) 的拥有两个元素的特殊情况。

一个 `pair` 保存两个数据成员。当创建一个 `pair` 时，我们必须提供两个数据成员的类型名，两个类型不要求一样。 `pair` 的数据成员是public的，两个成员分别命名为 `first` 和 `second` 。

当 `pair` 的数据成员 `first` 和 `second` 分别相等时，两个 `pair` 相等。

```c++
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
		cout << p.first << ":" << p.second << endl;			// pair的数据成员first和second是public的

	return 0;
}
```

