#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

struct Connection
{
	Connection(string ip, short port) :ip_(ip), port_(port) {}
	string ip_;
	short port_;
};

struct Destination
{
	Destination(string ip, short port) :ip_(ip), port_(port) {}
	string ip_;
	short port_;
};

Connection Connect(Destination des)
{
	Connection conn = Connection(des.ip_, des.port_);
	cout << "create connection:\t" << conn.ip_ << ":" << conn.port_ << endl;
	return conn;
}

void Disconnect(Connection conn)
{
	cout << "close connection:\t" << conn.ip_ << ":" << conn.port_ << endl;
}

void EndConnection(Connection* ptr)
{
	Disconnect(*ptr);
}

void Foo(Destination des)
{
	Connection conn = Connect(des);
	//unique_ptr<Connection, decltype(EndConnection)*> uptr(&conn, EndConnection);
	auto del = [](Connection* ptr) { Disconnect(*ptr); };
	unique_ptr<Connection, decltype(del)> uptr(&conn, del);
	cout << "connecting now:\t\t" << conn.ip_ << ":" << conn.port_ << endl;
}

int main()
{
	Destination des("202.118.176.67", 3316);
	Foo(des);

	auto del = [](int* p) { delete p; cout << "deleter" << endl; };
	unique_ptr<int, decltype(del)> up(new int(42), del);

	return 0;
}

/* 输出：
create connection:      202.118.176.67:3316
connecting now:         202.118.176.67:3316
close connection:       202.118.176.67:3316
deleter
*/

// tips: 
//   1. unique_ptr自定义删除器需要单个unique_ptr内部指针类型的参数