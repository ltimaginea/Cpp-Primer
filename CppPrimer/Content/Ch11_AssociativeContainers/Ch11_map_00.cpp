#include <iostream>
#include <map>
#include <string>
#include <utility>

using namespace std;

int main()
{
	map<string, size_t> word_count = { {"the",11},{"but",31},{"and",7},{"or",1},{"an",1},{"a",1} };

	auto map_it = word_count.begin();
	cout << (*map_it).first << " " << (*map_it).second << endl;
	//map_it->first = "A";			// 错误！map关键字是 const 的
	++map_it->second;				// 正确！可以改变map值

	word_count.erase("an");
	word_count.emplace("An", 99);

	word_count.insert({ "spring",3 });
	word_count.insert(pair<string, size_t>("summer", 6));
	word_count.insert(make_pair("autumn", 9));
	word_count.insert(map<string, size_t>::value_type("winter", 12));
	string word;
	while (cin >> word)
	{
		auto ret = word_count.insert({ word, 1 });		// 若word已在map中，则什么都不做，特别是与word相关联的计数器的值不变
		if (!ret.second)
			++(ret.first->second);						// 因为关键字重复而插入失败时，递增对应的计数器
	}

	word_count["Anna"] = 1;			// 如果关键字不在map中，则会添加一个具有此关键字的元素到map中
	cout << word_count["Anna"] << endl;
	++word_count["Anna"];
	cout << word_count["Anna"] << endl;

	map_it = word_count.find("spring");
	if (map_it != word_count.end())
		cout << map_it->first << " " << map_it->second << endl;
	cout << word_count.count("winter") << endl;

	cout << word_count.size() << endl;
	for (const auto& w : word_count)
		cout << w.first << " occurs " << w.second << ((w.second > 1) ? " times" : " time") << endl;
	word_count.clear();
	cout << word_count.size() << endl;
	return 0;
}
