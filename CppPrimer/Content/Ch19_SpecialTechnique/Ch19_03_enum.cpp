#include <iostream>

using namespace std;

enum Tokens { INLINE = 128, VIRTUAL = 129 };
void ff(Tokens t) { cout << t << endl; }
void ff(int t) { cout << t << endl; }

int main()
{
	// 限定作用域的枚举类型
	enum class open_modes { input, output, append };
	// 不限定作用域的枚举类型
	enum color { red, yellow, green };
	// 匿名，不限定作用域的枚举类型，同时在末尾定义它的对象 prec
	enum { intPrec = 6, floatPrec = -8, doublePrec, other = 6 } prec;		// 枚举值依次为 6,-8,-7,6

	// 错误！限定作用域的枚举必须有名称，不能匿名
	// enum class { input, output = -23, append };

	// 错误！重复定义了枚举成员 red, yellow, green
	// enum stoplight { red, yellow, green };

	enum class peppers { red, yellow, green };

	color eyes = green;
	// 正确！允许显式地访问枚举成员
	color hair = color::red;
	// 正确！使用 peppers 的 red
	peppers p = peppers::red;

	// 错误！peppers 的枚举成员 green 不在有效的作用域中；color::green 在有效的作用域中，但是类型错误
	// peppers p2 = green;

	// 错误！2 不属于枚举类型 peppers
	// peppers p3 = 2;

	int i = color::yellow;
	// 不能直接赋值给整型，需要显式类型转换
	int j = static_cast<int>(peppers::yellow);

	enum values :unsigned long long
	{
		charTyp = 255,
		shortTyp = 65535,
		intTyp = 65535,
		longTyp = 4294967295UL,
		long_longTyp = 18446744073709551615ULL
	};

	// 不限定作用域的枚举类型的前置声明，其必须指定成员类型
	enum intValues :unsigned long long;
	// 限定作用域的枚举类型的前置声明，可以使用默认成员类型 int
	enum class date;

	Tokens curTok = INLINE;
	ff(128);			// 精确匹配 void ff(int);
	ff(INLINE);			// 精确匹配 void ff(Tokens);
	ff(curTok);			// 精确匹配 void ff(Tokens);

	return 0;
}