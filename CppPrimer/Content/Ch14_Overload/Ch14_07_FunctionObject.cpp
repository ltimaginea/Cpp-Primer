#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class PrintString
{
public:
	PrintString(ostream& os = cout, char sep = ' ') :os_(os), sep_(sep) {  }
	void operator()(const string s) const
	{
		os_ << s << sep_;
	}
private:
	ostream& os_;
	char sep_;
};

int main()
{
	string s = "20210408";
	PrintString printer;
	printer(s);
	PrintString errors(cerr, '\n');
	errors(s);

	vector<string> vs = { "2021","04","08","21","11","59" };
	for_each(vs.begin(), vs.end(), PrintString());
	for_each(vs.begin(), vs.end(), PrintString(cerr, '\n'));
	for_each(vs.begin(), vs.end(), printer);

	return 0;
}

// tips: 
//   1. 第30和31行，for_each的第三个实参是类型PrintString的一个临时对象，其中我们用默认构造函数或者cerr和换行符初始化了该对象。