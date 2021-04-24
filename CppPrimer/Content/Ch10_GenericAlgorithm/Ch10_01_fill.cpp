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
