#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	vector<int> v(10);
	
	fill(v.begin(), v.end(), 233);
	for (auto elem : v)
		cout << elem << " " << endl;

	fill(v.begin(), v.end() - v.size() / 2, -1);
	for (auto elem : v)
		cout << elem << " " << endl;
	
	return 0;
}

// tips:
//	1. 常量对象，以及常量对象的引用或指针都只能调用常量成员函数。