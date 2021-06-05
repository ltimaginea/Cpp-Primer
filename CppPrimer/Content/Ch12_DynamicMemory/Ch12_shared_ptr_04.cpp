#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

class StrBlob
{
public:
	using SizeType = vector<string>::size_type;
	StrBlob() :data(make_shared<vector<string>>()) {}
	StrBlob(initializer_list<string> il) :data(make_shared<vector<string>>(il)) {}
	SizeType Size() const { return data->size(); }
	bool Empty() const { return data->empty(); }
	void PushBack(const string& t) { data->push_back(t); }
	void PopBack()
	{
		Check(0, "pop_back on empty StrBlob");
		data->pop_back();
	}
	string& Front()
	{
		Check(0, "front on empty StrBlob");
		return data->front();
	}
	const string& Front() const
	{
		Check(0, "front on empty StrBlob");
		return data->front();
	}
	string& Back()
	{
		Check(0, "back on empty StrBlob");
		return data->back();
	}
	const string& Back() const
	{
		Check(0, "back on empty StrBlob");
		return data->back();
	}
private:
	void Check(SizeType i, const string& msg) const
	{
		if (i >= data->size())
		{
			throw out_of_range(msg);
		}
	}
private:
	shared_ptr<vector<string>> data;
};

int main()
{
	const StrBlob csb = { "hello","world","cpp" };
	StrBlob sb = { "hello","world","java" };
	cout << csb.Front() << " " << csb.Back() << endl;
	sb.Back() = "python";
	cout << sb.Front() << " " << sb.Back() << endl;
	return 0;
}

// tips: 
//   1. pop_back,front和back在试图访问元素之前必须检查元素是否存在