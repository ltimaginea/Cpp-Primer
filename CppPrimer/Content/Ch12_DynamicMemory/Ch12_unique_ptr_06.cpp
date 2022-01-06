#include <iostream>
#include <vector>
#include <string>
#include <memory>

struct Foo
{
	int id;
	Foo(int id = 0) : id(id) { std::cout << "Foo " << id << '\n'; }
	~Foo() { std::cout << "~Foo " << id << '\n'; }
};

class Song
{
public:
	std::wstring title_;
	std::wstring artist_;

	Song(std::wstring title, std::wstring artist) : title_(title), artist_(artist) {  }
	Song(std::wstring title) : title_(title), artist_(L"Unknown") {  }
};

void SongVector()
{
	using std::vector;
	using std::unique_ptr;
	using std::make_unique;

	vector<unique_ptr<Song>> songs;

	// Create a few new unique_ptr<Song> instances and add them to vector using implicit move semantics.
	songs.push_back(make_unique<Song>(L"B'z", L"Juice"));
	songs.push_back(make_unique<Song>(L"Namie Amuro", L"Funky Town"));
	songs.push_back(make_unique<Song>(L"Kome Kome Club", L"Kimi ga Iru Dake de"));
	songs.push_back(make_unique<Song>(L"Ayumi Hamasaki", L"Poker Face"));

	// In the range for loop, notice that the unique_ptr is passed by reference. 
	// If you try to pass by value here, the compiler will throw an error because the unique_ptr copy constructor is deleted.
	// Pass by const reference when possible to avoid copying.
	for (const auto& song : songs)
	{
		std::wcout << L"Artist: " << song->artist_ << L"   Title: " << song->title_ << std::endl;
	}
}

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
	// safe copy!
	unique_ptr<string> up6(up2 ? make_unique<string>(*up2) : nullptr);

	up1.reset(up2.release());
	// 接管右侧新资源，释放左侧旧资源，可以认为等效于上一句
	up3 = std::move(up4);
	// copy!
	up5 = make_unique<string>(*up3);	// 无需再通过std::move将make_unique的返回值强制类型转换为右值，因为make_unique的返回值是临时对象，即右值
	// safe copy!
	up5 = up3 ? make_unique<string>(*up3) : nullptr;
	//up5.reset(make_unique<string>(*up3).release());		// 同上等效
	//up5.reset(new string(*up3));		// 同上等效

	// 不建议下面这样赋值，其安全性依赖于*up的类型的operator=的实现，安全性不稳定
	// *up1 = *up5;

	unique_ptr<Foo> p1(make_unique<Foo>(1));

	{
		std::cout << "Creating new Foo...\n";
		unique_ptr<Foo> p2(make_unique<Foo>(2));
		// p1 = p2; // Error ! can't copy unique_ptr
		p1 = std::move(p2);
		std::cout << "About to leave inner block...\n";

		// Foo instance will continue to live, despite p2 going out of scope
	}

	// Create a unique_ptr to an array of 5 Foos.
	unique_ptr<Foo[]> p_array = make_unique<Foo[]>(5);

	{
		std::cout << "Creating new Foo []...\n";
		p_array = make_unique<Foo[]>(3);
		std::cout << "About to leave inner block []...\n";
	}

	// 错误！类型不匹配
	// p1 = std::move(p_array);

	SongVector();

	// We can use make_unique to create a unique_ptr to an array, but cannot use make_unique to initialize the array elements.
	// We have to initialize the array elements separately. Rather than using make_unique<T[]>, perhaps a better choice is to use a std::vector.
	// Create a unique_ptr to an array of 5 integers.
	auto p = make_unique<int[]>(5);
	// Initialize the array elements separately.
	for (int i = 0; i < 5; ++i)
	{
		p[i] = i;
		std::cout << p[i] << " ";
	}
	std::cout << "\n";

	std::cout << "About to leave program...\n";
	return 0;
}

// Tips:
//  1. unique_ptr的移动赋值，接管新资源的同时，还会销毁旧资源，如果有的话

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
Artist: Juice   Title: B'z
Artist: Funky Town   Title: Namie Amuro
Artist: Kimi ga Iru Dake de   Title: Kome Kome Club
Artist: Poker Face   Title: Ayumi Hamasaki
0 1 2 3 4
About to leave program...
~Foo 0
~Foo 0
~Foo 0
~Foo 2
*/