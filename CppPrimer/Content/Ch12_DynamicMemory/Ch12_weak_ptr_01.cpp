#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <initializer_list>

using namespace std;

class StrBlobPtr;

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
public:
	// 友元声明
	friend class StrBlobPtr;
	// 返回指向首元素的StrBlobPtr
	StrBlobPtr Begin() const;
	// 返回指向尾后元素的StrBlobPtr
	StrBlobPtr End() const;
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

class StrBlobPtr
{
public:
	StrBlobPtr() :curr(0) {}
	StrBlobPtr(const StrBlob& a, size_t sz = 0) :wptr(a.data), curr(sz) {}
	// 解引用
	string& Deref()
	{
		auto p = Check(curr, "dereference past end");
		return (*p)[curr];
		//return p->operator[](curr);		// 等价
		//return (*p).operator[](curr);		// 等价
	}
	const string& Deref() const
	{
		auto p = Check(curr, "dereference past end");
		return (*p)[curr];
	}
	// 前缀递增，返回递增后的对象的引用
	StrBlobPtr& Incr()
	{
		// 如果curr已经指向容器的尾后位置，就不能递增它
		Check(curr, "increment past end of StrBlobPtr");
		// 推进当前位置
		++curr;
		return *this;
	}
private:
	// 若check成功，则返回一个shared_ptr<vector<string>>
	shared_ptr<vector<string>> Check(size_t i, const string& msg) const
	{
		auto ret = wptr.lock();
		if (!ret)
		{
			throw runtime_error("unbound StrBlobPtr");
		}
		else if (i >= ret->size())
		{
			throw out_of_range(msg);
		}
		return ret;
	}
private:
	weak_ptr<vector<string>> wptr;
	// 数组中的当前位置
	size_t curr;
};

// 返回指向首元素的StrBlobPtr
StrBlobPtr StrBlob::Begin() const
{
	return StrBlobPtr(*this);
}
// 返回指向尾后元素的StrBlobPtr
StrBlobPtr StrBlob::End() const
{
	auto ret = StrBlobPtr(*this, data->size());
	return ret;
}

int main()
{
	const StrBlob csb = { "hello","world","cpp" };
	StrBlob sb = { "hello","world","java" };
	
	StrBlobPtr sbp(sb.Begin());
	cout << sbp.Deref() << endl;
	sbp.Incr();
	cout << sbp.Deref() << endl;
	
	const StrBlobPtr csbp(csb.Begin());
	cout << csbp.Deref() << endl;
	
	sbp = sb.End();
	// 解引用会抛异常，不明白《C++ Primer》中定义 StrBlobPtr StrBlob::End() 的意义
	//cout << sbp.Deref() << endl;

	return 0;
}

// tips: 
//   1. pop_back,front和back在试图访问元素之前必须检查元素是否存在