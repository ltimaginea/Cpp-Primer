#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern double pd;

int main()
{
	int a, b(1), c{ 2 }, d = 3, e = { 4 };
	
	// 在函数体内部，如果试图初始化一个由extern关键字标记的变量，将引发错误
	// extern double pd0 = 3.1416;
	
	// 空string
	string s0;
	string s1("hiya");
	// hhhhhhhhhh
	string s2(10, 'h');
	vector<string> s3{ "a","an","the" };
	string s4 = "hiyou";
	vector<string> s5 = { "one","two","three" };

	return 0;
}

// tips: 
//   1. 在函数体内部，如果试图初始化一个由extern关键字标记的变量，将引发错误。