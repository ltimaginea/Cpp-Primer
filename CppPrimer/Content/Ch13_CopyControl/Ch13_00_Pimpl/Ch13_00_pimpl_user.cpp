#include "Ch13_00_pimpl_widget.h"

#include <iostream>
#include <vector>
#include <utility>
#include <type_traits>

int main()
{
	// Output: false
	std::cout << std::boolalpha << std::is_nothrow_move_constructible<Widget>::value << std::noboolalpha << std::endl;
	// Output: false
	std::cout << std::boolalpha << std::is_nothrow_move_assignable<Widget>::value << std::noboolalpha << std::endl;

	Widget w0;
	Widget w1(1), w2(2), w3(3);
	Widget w4(w1);
	Widget w5(std::move(w1));
	w3 = w2;
	w3 = std::move(w2);

	const Widget w6(6);
	w6.Draw(); w6.PrintInfo();
	w5.Draw(); w5.PrintInfo();

	w0.PrintInfo(); // Do nothing because a default constructed object's pimpl_ is a null pointer.
	w1.PrintInfo(); // Do nothing because a moved-from object's pimpl_ is a null pointer.
	w2.PrintInfo(); // Do nothing because a moved-from object's pimpl_ is a null pointer.
	w3.PrintInfo();
	w4.PrintInfo();
	w5.PrintInfo();

	{
		Widget w1(11);
		Widget w2(22);
		Widget w3(33);
		Widget w4(44);
		std::vector<Widget> v;
		// 注释掉下面这句，便可以观察到vector重新分配内存的过程中，使用了类Widget的拷贝构造函数来把旧元素复制到新内存中，而并没有使用类Widget的移动构造函数来把旧元素移动到新内存中，
		// 原因是类Widget的合成移动构造函数没有承诺不会抛出异常，详见类Widget的合成移动构造函数的注释。
		// v.reserve(10);
		v.push_back(std::move(w1));
		v.push_back(std::move(w2));
		v.push_back(std::move(w3));
		v.push_back(std::move(w4));
	}

	return 0;
}

/* Outputs:
false
false
drawing a const widget 6
6
drawing a non-const widget 1
1
2
1
1
*/
