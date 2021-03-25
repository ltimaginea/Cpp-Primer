#include <iostream>
#include <string>
#include <map>

#include <functional>

using namespace std;

int add(int i, int j)
{
	return i + j;
}

string add(const string& str1, const string& str2)
{
	return str1 + str2;
}

int main()
{
	map<string, function<string(const string&, const string&)>> binops;
	//binops.insert({ "+", add });        // 错误! 无法确定需要哪个重载函数"add"实例

	// 存储函数指针
	string(*fp)(const string&, const string&) = add;
	binops.insert({ "+",fp });
	cout << binops["+"]("fp_Overload", "Function") << endl;

	// 强制转型到适当的函数指针型别
	using AddString = string(*)(const string&, const string&);
	binops.insert({ "+",static_cast<AddString>(add) });
	cout << binops["+"]("static_cast_Overload", "Function") << endl;

	// 借助lambda
	binops.insert({ "+",[](const string& str1, const string& str2) { return add(str1,str2); } });
	cout << binops["+"]("lambda_Overload", "Function") << endl;

	return 0;
}

// tips: 
//   1. 我们不能直接将重载函数的名字存入 function 类型的对象中。
//      解决二义性的途径: 存储函数指针或强制转型到适当的函数指针型别，也可以借助lambda来消除二义性。