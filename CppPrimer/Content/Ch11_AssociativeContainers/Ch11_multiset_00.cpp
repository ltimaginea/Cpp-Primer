#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <utility>

using namespace std;

int main()
{
	multiset<string> exclude = { "The","The","The","But","But","And","And","Or","Or","An","An","A","A" };
	auto it = exclude.begin();
	cout << *it << endl;
	//*it = "AAA";			// 错误！multiset关键字是 const 的

	auto cnt = exclude.erase("The");
	cout << "erase num is " << cnt << endl;
	exclude.emplace("THE");
	exclude.emplace("THE");
	exclude.emplace("THE");

	vector<string> vstr = { "spring" ,"summer","autumn","winter","spring" ,"summer","autumn","winter" };	// 实际只会插入前4个元素，重复的后4个不会被插入
	exclude.insert(vstr.begin(), vstr.end());
	string word;
	while (cin >> word)
	{
		auto ret = exclude.insert(word);		// multiset总是添加一个新元素
		cout << "insert " << *ret << endl;
	}

	//exclude["Anna"] = 1;			// multiset不支持下标运算符

	string item = "spring";
	it = exclude.find(item);
	cnt = exclude.count(item);
	while (cnt)
	{
		cout << *it << endl;
		++it;
		--cnt;
	}

	item = "summer";
	for (auto beg = exclude.lower_bound(item), end = exclude.upper_bound(item); beg != end; ++beg)
	{
		cout << *beg << endl;
	}

	item = "THE";
	for (auto r = exclude.equal_range(item); r.first != r.second; ++(r.first))
	{
		cout << *(r.first) << endl;
	}

	cout << exclude.size() << endl;
	for (const auto& e : exclude)
		cout << e << endl;
	exclude.clear();
	cout << exclude.size() << endl;

	return 0;
}

// tips: 
//   1. multiset不支持下标运算符。