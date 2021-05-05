#include <iostream>
#include <unordered_set>
#include <string>
#include <vector>
#include <utility>

using namespace std;

int main()
{
	unordered_multiset<string> exclude = { "The","The","The","But","But","And","And","Or","Or","An","An","A","A" };
	auto it = exclude.begin();
	cout << *it << endl;
	//*it = "AAA";			// 错误！unordered_multiset关键字是 const 的

	auto cnt = exclude.erase("The");
	cout << "erase num is " << cnt << endl;
	exclude.emplace("THE");
	exclude.emplace("THE");
	exclude.emplace("THE");

	vector<string> vstr = { "spring" ,"summer","autumn","winter","spring" ,"summer","autumn","winter" };
	exclude.insert(vstr.begin(), vstr.end());
	string word;
	while (cin >> word)
	{
		auto ret = exclude.insert(word);		// unordered_multiset总是添加一个新元素
		cout << "insert " << *ret << endl;
	}

	//exclude["Anna"] = 1;			// unordered_multiset不支持下标运算符

	string item = "spring";
	it = exclude.find(item);
	cout << "find " << *it << endl;
	cnt = exclude.count(item);
	cout << "spring num is " << cnt << endl;

	cout << exclude.size() << endl;
	for (const auto& e : exclude)
		cout << e << endl;
	exclude.clear();
	cout << exclude.size() << endl;

	return 0;
}

// tips: 
//   1. unordered_multiset不支持下标运算符。