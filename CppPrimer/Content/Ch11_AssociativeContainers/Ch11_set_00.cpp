#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <utility>

using namespace std;

int main()
{
	set<string> exclude = { "The","But","And","Or","An","A","the","but","and","or","an","a" };
	auto it = exclude.begin();
	cout << *it << endl;
	//*it = "AAA";			// 错误！set关键字是 const 的

	exclude.erase("The");
	exclude.emplace("THE");

	vector<string> vstr = { "spring" ,"summer","autumn","winter","spring" ,"summer","autumn","winter" };	// 实际只会插入前4个元素，重复的后4个不会被插入
	exclude.insert(vstr.begin(), vstr.end());
	string word;
	while (cin >> word)
	{
		auto ret = exclude.insert(word);		// 若word已存在set中，则什么都不做
		if (!ret.second)
			cout << *(ret.first) << " key is exited" << endl;
		else
			cout << *(ret.first) << " insert successfully" << endl;
	}

	//exclude["Anna"] = 1;			// set不支持下标运算符

	it = exclude.find("spring");
	if (it != exclude.end())
		cout << *it << endl;
	cout << exclude.count("winter") << endl;

	cout << exclude.size() << endl;
	for (const auto& e : exclude)
		cout << e << endl;
	exclude.clear();
	cout << exclude.size() << endl;

	return 0;
}

// tips: 
//   1. set不支持下标运算符。