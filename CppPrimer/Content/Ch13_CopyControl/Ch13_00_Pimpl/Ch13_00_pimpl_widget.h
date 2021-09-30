#pragma once
#include <memory>

class Widget
{
public:
	Widget(int i = 0);
	Widget(const Widget&);
	Widget(Widget&&);
	Widget& operator=(const Widget&);
	Widget& operator=(Widget&&);
	~Widget();
public:
	void PrintInfo();
	// ...
private:
	// things to be hidden go here
	class Impl;
	// opaque pointer to forward-declared class
	std::unique_ptr<Impl> pimpl_;
};

// Tips:
//	1. ���� std::unique_ptr ���ԣ�ɾ����������������ָ�����͵�һ����
//	2. ���� std::shared_ptr ���ԣ�ɾ���������Ͳ�������ָ�����͵�һ����