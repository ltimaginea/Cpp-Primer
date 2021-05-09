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
		return ((m_ == a.m_) && (n_ == a.n_));
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
	// 实参分别是桶大小和哈希函数指针
	unordered_multiset<A, decltype(Hasher)*> s1(128, Hasher);
	s1 = { A(2,3),A(2,3),A(-12,12),A(-12,12),A(19,-9),A(19,-9) };
	for (const auto& s : s1)
		cout << s << endl;
	cout << s1.bucket_count() << endl;
	cout << s1.max_bucket_count() << endl << endl;
	
	// 实参分别是桶大小、哈希函数指针和相等性判断运算符指针
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
	// 猜测: 如果两个关键字相等，那么它们的哈希值计算出来也应该相等，最终便会存储在同一个桶中。
	//      但是s6这里，哈希值的计算方法和相等性判断方法不一致（计算和比较的数据不一致），最后哈希计算后认为不相等（哈希值不相等），但是相等性判断方法认为相等，
	//      这样的矛盾最终导致了A(501, 67)和A(301, 67)没有保存在同一个桶中。下面《C++ Primer》中的描述也算是佐证吧。
	// 参考：无序容器用哈希技术而不是比较操作来管理元素的位置、存储和访问元素。——《C++ Primer》
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
//   1. 实参分别是桶大小、哈希函数指针和相等性判断运算符指针。
//   2. 定义 s1和s2 的写法为《C++ Primer》中的示例写法。
//   3. 我们可以自定义比较操作，其可以是函数对象类，也可以是函数指针类型，或对"=="运算符经过适当重载。
//   4. 当自定义的比较函数是成员函数时，需要把比较函数定义为const成员函数。
//   5. 当比较操作是函数指针类型时，必须用具体的函数指针来初始化对象。
