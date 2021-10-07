#include <vector>

#include "Ch13_00_pimpl_widget.h"

int main()
{
	Widget w1(1);
	Widget w2(w1);
	Widget w3 = std::move(w1);
	Widget w4(4);
	w1 = w4;
	Widget w5(5);
	Widget w6(6);
	w5 = std::move(w5);
	w6 = w1;

	w1.PrintInfo();
	w2.PrintInfo();
	w3.PrintInfo();
	w4.PrintInfo();
	w5.PrintInfo();
	w6.PrintInfo();

	{
		Widget w1(11);
		Widget w2(22);
		Widget w3(33);
		Widget w4(44);
		std::vector<Widget> v;
		//v.reserve(10);	// ע�͵���䣬����Թ۲쵽vector���·����ڴ�����еġ��ƶ�/������Ԫ�������ڴ桱
		v.push_back(std::move(w1));
		v.push_back(std::move(w2));
		v.push_back(std::move(w3));
		v.push_back(std::move(w4));
	}

	return 0;
}

/*
Outputs:
4
1
1
4
5
4
*/
