#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	vector<int> v(10, 233);

	auto n = v.size() / 2;
	fill_n(v.begin(), n, -1);		// 从目的位置开始的序列应至少包含n个元素，要能容纳要写入的元素
	for (auto elem : v)
		cout << elem << " " << endl;
	
	return 0;
}

// tips:
//	1. 从目的位置开始的序列应至少包含n个元素，要能容纳要写入的元素。