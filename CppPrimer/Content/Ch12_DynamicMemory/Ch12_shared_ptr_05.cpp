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
	//shared_ptr<Connection> sptr(&conn, EndConnection);
	shared_ptr<Connection> sptr(&conn, [](Connection* ptr) { Disconnect(*ptr); });
	cout << "connecting now:\t\t" << conn.ip_ << ":" << conn.port_ << endl;
}

int main()
{
	Destination des("202.118.176.67", 3316);
	Foo(des);

	shared_ptr<int> sp(new int(42), [](int* p) { delete p; cout << "deleter" << endl; });

	return 0;
}

/* 输出：
create connection:      202.118.176.67:3316
connecting now:         202.118.176.67:3316
close connection:       202.118.176.67:3316
deleter
*/

// tips: 
//   1. shared_ptr自定义删除器需要单个shared_ptr内部指针类型的参数