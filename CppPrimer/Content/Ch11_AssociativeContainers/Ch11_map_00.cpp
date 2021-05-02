#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
	map<string, size_t> word_count = { {"the",11},{"but",31},{"and",7},{"or",23},{"an",12},{"a",1} };
	string word;
	while (cin >> word)
	{
		auto ret = word_count.insert({ word,1 });
		if (!ret.second)
			++word_count[word];
	}
	for (auto m : word_count)
		cout << m.first << "\t" << m.second << endl;
	return 0;
}
