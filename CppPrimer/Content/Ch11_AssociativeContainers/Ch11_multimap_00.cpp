#include <iostream>
#include <map>
#include <string>
#include <utility>

using namespace std;

int main()
{
	multimap<string, size_t> m_word_count = { {"Apple",5},{"Apple",5},{"Tiger",555} ,{"Tiger",555} };
	{
		map<string, size_t> word_count = { {"the",12},{"but",33},{"and",7},{"or",1},{"an",1},{"a",1} };
		for (const auto& w : word_count)
		{
			m_word_count.insert(w);
			m_word_count.insert(w);
		}
	}
	for (const auto& m_w : m_word_count)
		cout << m_w.first << " " << m_w.second << endl;

	auto it = m_word_count.begin();
	cout << (*it).first << " " << (*it).second << endl;
	//it->first = "A";			// 错误！multimap关键字是 const 的
	++it->second;				// 正确！可以改变multimap值

	auto cnt = m_word_count.erase("an");
	cout << "erase num is " << cnt << endl;
	m_word_count.emplace("An", 99);
	m_word_count.emplace("An", 99);
	m_word_count.emplace("An", 89);

	m_word_count.insert({ "spring",93 });
	m_word_count.insert({ "spring",73 });
	m_word_count.insert(pair<string, size_t>("summer", 6));
	m_word_count.insert(pair<string, size_t>("summer", 76));
	m_word_count.insert(make_pair("autumn", 9));
	m_word_count.insert(make_pair("autumn", 79));
	m_word_count.insert(map<string, size_t>::value_type("winter", 12));
	m_word_count.insert(map<string, size_t>::value_type("winter", 712));
	string word;
	while (cin >> word)
	{
		auto ret = m_word_count.insert({ word, 1 });		// multimap总是添加一个新元素
		cout << "insert " << ret->first << " " << ret->second << endl;
	}

	//m_word_count["Anna"] = 1;			// multimap不支持下标运算符

	string item = "spring";
	it = m_word_count.find(item);
	cnt = m_word_count.count(item);
	while (cnt)
	{
		cout << it->first << " " << it->second << endl;
		++it;
		--cnt;
	}

	item = "An";
	for (auto r = m_word_count.equal_range(item); r.first != r.second; ++r.first)
		cout << r.first->first << " " << r.first->second << endl;

	item = "summer";
	for (auto beg = m_word_count.lower_bound(item), end = m_word_count.upper_bound(item); beg != end; ++beg)
		cout << beg->first << " " << beg->second << endl;

	for (const auto& m_w : m_word_count)
		cout << m_w.first << " " << m_w.second << endl;
	m_word_count.clear();
	cout << m_word_count.size() << endl;
	return 0;
}

// tips: 
//   1. multimap不支持下标运算符。