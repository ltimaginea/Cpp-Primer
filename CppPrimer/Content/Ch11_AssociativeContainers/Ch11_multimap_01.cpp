﻿#include <iostream>
#include <utility>
#include <map>
#include <string>

using namespace std;

class A
{
public:
	A(int m, int n) :m_(m), n_(n) {}
	~A() {};

	bool operator<(const A& a) const			// 必须是const成员函数
	{
		return m_ < a.m_;
	}

	friend bool MyLess(const A& a, const A& b);
	friend class MyGreater;
	friend ostream& operator<<(ostream& os, const A& a);

private:
	int m_;
	int n_;
};

bool MyLess(const A& a, const A& b)
{
	return a.n_ < b.n_;
}

class MyGreater
{
public:
	bool operator()(const A& a, const A& b) const		// 必须是const成员函数
	{
		return a.n_ % 10 > b.n_ % 10;
	}
};

ostream& operator<<(ostream& os, const A& a)
{
	os << a.m_ << " " << a.n_;
	return os;
}

int main()
{
	multimap<A, string> m1 = { {A(2,3),"2,3"},{A(2,3),"2,3,2,3"}, {A(-12,12),"-12,12,-12,12"}, {A(-12,12),"-12,12"} ,{A(19,-9),"19,-9"} ,{A(19,-9),"198,-9"} };
	for (const auto& m : m1)
		cout << m.first << "\t" << m.second << endl;
	cout << endl;

	multimap<A, string, MyGreater> m2 = { {A(2,3),"2,3"},{A(2,3),"2,3,2,3"}, {A(-12,12),"-12,12,-12,12"}, {A(-12,12),"-12,12"} ,{A(19,-9),"19,-9"} ,{A(19,-9),"198,-9"} };		// function object class as Compare
	for (const auto& m : m2)
		cout << m.first << "\t" << m.second << endl;
	cout << endl;

	multimap<A, string, decltype(MyLess)*> m3(MyLess);		// 当比较操作是函数指针类型时，必须用具体的函数指针来初始化对象，这里用MyLess来初始化m3
	m3 = { {A(2,3),"2,3"},{A(2,3),"2,3,2,3"}, {A(-12,12),"-12,12,-12,12"}, {A(-12,12),"-12,12"} ,{A(19,-9),"19,-9"} ,{A(19,-9),"198,-9"} };
	for (const auto& m : m3)
		cout << m.first << "\t" << m.second << endl;
	cout << endl;

	using MyLessType = bool (*)(const A&, const A&);
	multimap<A, string, MyLessType> m4(MyLess);				// 当比较操作是函数指针类型时，必须用具体的函数指针来初始化对象，这里用MyLess来初始化m4
	m4.insert(pair<A, string>(A(-100, 100), "-100,100"));
	m4.insert(pair<A, string>(A(301, 67), "301,67"));
	m4.insert(pair<A, string>(A(501, 67), "301,67"));		// 根据自定义的比较操作，关键字A(501, 67)和A(301, 67)是相等的

	for (const auto& m : m4)
		cout << m.first << "\t" << m.second << endl;
	cout << endl;

	return 0;
}

// tips: 
//   1. 我们可以自定义比较操作，其可以是函数对象类，也可以是函数指针类型，或对"<"运算符经过适当重载。
//   2. 当自定义的比较函数是成员函数时，需要把比较函数定义为const成员函数。
//   3. 当比较操作是函数指针类型时，必须用具体的函数指针来初始化对象。
