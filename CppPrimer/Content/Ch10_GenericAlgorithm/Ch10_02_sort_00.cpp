#include <iostream>
#include <string>
#include <array>
#include <algorithm>

using namespace std;

bool cmp(string a, string b)
{
	return a.length() > b.length();
}

int main()
{
	double d[] = { 1.4,-2.1,3.3,9.06,9.07 };
	
	sort(begin(d), end(d));		// 默认递增排序
	for_each(begin(d), end(d), [](double i) {cout << i << " "; });
	cout << endl;
	
	sort(begin(d), end(d), greater<double>());		// 递减排序
	for_each(begin(d), end(d), [](auto i) {cout << i << " "; });
	cout << endl;
	
	sort(begin(d), end(d), less<double>());			// 递增排序
	for_each(begin(d), end(d), [](auto i) {cout << i << " "; });
	cout << endl;



	array<string, 3> astr = { "bbbb","cc","aaa" };
	
	sort(astr.begin(), astr.end());					// 按照字典序排序
	for_each(astr.begin(), astr.end(), [](auto i) {cout << i << " "; });
	cout << endl;

	sort(astr.begin(), astr.end(), cmp);			// 按照字符串长度排序
	for_each(astr.begin(), astr.end(), [](auto i) {cout << i << " "; });
	cout << endl;

	return 0;
}
