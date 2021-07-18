#include <iostream>
#include <iomanip>

#include <math.h>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

int main()
{
	bool is = false;
	cout << boolalpha << is << noboolalpha << endl;

	int32_t sale = 343797;
	// 启用或禁用非负数输出中的正号 + 的显示
	cout << showpos << sale << noshowpos << endl;

	cout << showbase << uppercase << hex;
	cout << UINT32_MAX << endl;
	cout << noshowbase << nouppercase << dec;

	// 返回当前精度值
	auto prec = cout.precision();
	cout << prec << endl;
	cout << sqrt(2.0) << endl;
	cout.precision(12);
	cout << sqrt(2.0) << endl;
	cout << setprecision(7);
	cout << sqrt(2.0) << endl;
	cout.precision(prec);
	prec = cout.precision();
	cout << prec << endl;

	cout << 100 * sqrt(2.0) << endl;
	cout << fixed << 100 * sqrt(2.0) << endl;
	cout << scientific << 100 * sqrt(2.0) << endl;
	cout << hexfloat << 100 * sqrt(2.0) << endl;
	cout << defaultfloat << 100 * sqrt(2.0) << endl;

	cout << 10.0 << endl;
	cout << showpoint << 10.0 << noshowpoint << endl;

	int i = -16;
	double d = 3.14159;
	cout << i << endl;
	cout << d << endl;
	cout << setw(12) << i << endl;
	cout << setw(12) << d << endl;

	cout << left;
	cout << setw(12) << i << endl;
	cout << setw(12) << d << endl;

	cout << right;
	cout << setw(12) << i << endl;
	cout << setw(12) << d << endl;

	cout << internal;
	cout << setw(12) << i << endl;
	cout << setw(12) << d << endl;

	cout << setfill('#');
	cout << setw(12) << i << endl;
	cout << setw(12) << d << endl;
	cout << setfill(' ');

	char ch;
	// 设置读取空白符
	cin >> noskipws;
	while (cin >> ch)
		cout << ch;
	// 恢复到默认状态，忽略读取空白符
	cin >> skipws;

	return 0;
}