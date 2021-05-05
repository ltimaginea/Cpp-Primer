#include <iostream>
#include <utility>
#include <unordered_set>
#include <string>
#include <functional>

using namespace std;

class A
{
public:
	A(int m, int n) :m_(m), n_(n) {}
	~A() {}

	bool operator==(const A& a) const			// 必须是const成员函数
	{
		return m_ == a.m_;
	}

	friend bool MyEqual(const A& a, const A& b);
	friend class MyEqualClass;
	friend size_t Hasher(const A& a);
	friend class HasherClass;
	friend ostream& operator<<(ostream& os, const A& a);

private:
	int m_;
	int n_;
};

bool MyEqual(const A& a, const A& b)
{
	return a.n_ == b.n_;
}

class MyEqualClass
{
public:
	bool operator()(const A& a, const A& b) const		// 必须是const成员函数
	{
		return a.n_ % 10 == b.n_ % 10;
	}
};

ostream& operator<<(ostream& os, const A& a)
{
	os << a.m_ << " " << a.n_;
	return os;
}

size_t Hasher(const A& a)
{
	return hash<int>()(a.n_);
}

class HasherClass
{
public:
	size_t operator()(const A& a) const		// 必须是const成员函数
	{
		return hash<int>()(a.m_);
	}
};
HasherClass hasher;

int main()
{
	unordered_multiset<A, decltype(Hasher)*> s1(128, Hasher);
	s1 = { A(2,3),A(2,3),A(-12,12),A(-12,12),A(19,-9),A(19,-9) };
	for (const auto& s : s1)
		cout << s << endl;
	cout << s1.bucket_count() << endl;
	cout << s1.max_bucket_count() << endl << endl;

	unordered_multiset<A, decltype(Hasher)*, decltype(MyEqual)*> s2(42, Hasher, MyEqual);
	s2 = { A(2,3),A(2,3),A(-12,12),A(-12,12),A(19,-9),A(19,-9) };
	for (const auto& s : s2)
		cout << s << endl;
	cout << s2.bucket_count() << endl;
	cout << s2.max_bucket_count() << endl << endl;

	unordered_multiset<A, decltype(Hasher)*, MyEqualClass> s3(64, Hasher);
	s3 = { A(2,3),A(2,3),A(-12,12),A(-12,12),A(19,-9),A(19,-9) };
	for (const auto& s : s3)
		cout << s << endl;
	cout << s3.bucket_count() << endl;
	cout << s3.max_bucket_count() << endl << endl;

	unordered_multiset<A, HasherClass, MyEqualClass> s4 = { A(2,3),A(2,3),A(-12,12),A(-12,12),A(19,-9),A(19,-9) };		// function object class as Compare
	for (const auto& s : s4)
		cout << s << endl;
	cout << s4.bucket_count() << endl;
	cout << s4.max_bucket_count() << endl << endl;

	unordered_multiset<A, HasherClass, decltype(MyEqual)*> s5(8, HasherClass(), MyEqual);
	s5 = { A(2,3),A(2,3),A(-12,12),A(-12,12),A(19,-9),A(19,-9) };
	for (const auto& s : s5)
		cout << s << endl;
	cout << s5.bucket_count() << endl;
	cout << s5.max_bucket_count() << endl << endl;

	unordered_multiset<A, HasherClass, decltype(MyEqual)*> s6(10, hasher, MyEqual);
	s6.insert(A(301, 67));
	s6.insert(A(-100, 100));
	// 根据自定义的比较操作，关键字A(501, 67)和A(301, 67)相等，它们会保存在同一个桶中。但是测试s6，发现并没有保存在同一个桶中，暂时还不清楚这是为什么？
	s6.insert(A(501, 67));
	for (const auto& s : s6)
		cout << s << endl;
	cout << s6.bucket_count() << endl;
	cout << s6.max_bucket_count() << endl << endl;
	cout << s6.bucket(A(301, 67)) << endl;
	cout << s6.bucket(A(501, 67)) << endl;
	cout << s6.bucket(A(-100, 100)) << endl;

	return 0;
}

// tips: 
//   0. 定义 s2 的写法为《C++ Primer》中的示例写法。
//   1. 我们可以自定义比较操作，其可以是函数对象类，也可以是函数指针类型，或对"=="运算符经过适当重载。
//   2. 当自定义的比较函数是成员函数时，需要把比较函数定义为const成员函数。
//   3. 当比较操作是函数指针类型时，必须用具体的函数指针来初始化对象。
