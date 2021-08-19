#include <iostream>
#include <string>
#include <memory>

struct Foo
{
	int id;
	Foo(int id = 0) : id(id) { std::cout << "Foo " << id << '\n'; }
	~Foo() { std::cout << "~Foo " << id << '\n'; }
};

int main()
{
	using std::string;
	using std::unique_ptr;
	using std::make_unique;

	unique_ptr<string> up1(new string("Good morning"));
	unique_ptr<string> up2(make_unique<string>("sleep"));
	unique_ptr<string> up3 = make_unique<string>("weak up");
	unique_ptr<string> up4(std::move(up1));
	// copy!
	unique_ptr<string> up5(make_unique<string>(*up2));

	up1.reset(up2.release());
	// �ӹ��Ҳ�����Դ���ͷ�������Դ��������Ϊ��Ч����һ��
	up3 = std::move(up4);
	// copy!
	up5 = make_unique<string>(*up3);	// ������ͨ��std::move��make_unique�ķ���ֵǿ������ת��Ϊ��ֵ����Ϊmake_unique�ķ���ֵ����ʱ���󣬼���ֵ
	//up5.reset(make_unique<string>(*up3).release());		// ͬ�ϵ�Ч
	//up5.reset(new string(*up3));		// ͬ�ϵ�Ч

	// ����������������ֵ���䰲ȫ��������*up�����͵�operator=��ʵ�֣���ȫ�Բ�ȷ��
	*up1 = *up5;

	unique_ptr<Foo> p1(make_unique<Foo>(1));

	{
		std::cout << "Creating new Foo...\n";
		unique_ptr<Foo> p2(make_unique<Foo>(2));
		// p1 = p2; // Error ! can't copy unique_ptr
		p1 = std::move(p2);
		std::cout << "About to leave inner block...\n";

		// Foo instance will continue to live, despite p2 going out of scope
	}

	// create a unique_ptr to an array of 5 Foos.
	unique_ptr<Foo[]> p_array = make_unique<Foo[]>(5);

	{
		std::cout << "Creating new Foo []...\n";
		p_array = make_unique<Foo[]>(3);
		std::cout << "About to leave inner block []...\n";
	}

	//p1 = std::move(p_array);

	std::cout << "About to leave program...\n";
	return 0;
}

// Tips:
//  1. unique_ptr���ƶ���ֵ���ӹ�����Դ��ͬʱ���������پ���Դ������еĻ�

/*
Outputs:
Foo 1
Creating new Foo...
Foo 2
~Foo 1
About to leave inner block...
Foo 0
Foo 0
Foo 0
Foo 0
Foo 0
Creating new Foo []...
Foo 0
Foo 0
Foo 0
~Foo 0
~Foo 0
~Foo 0
~Foo 0
~Foo 0
About to leave inner block []...
About to leave program...
~Foo 0
~Foo 0
~Foo 0
~Foo 2
*/