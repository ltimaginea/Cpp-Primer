#include "Ch07_06_StaticMembers.h"

namespace staticmembers
{
	void Account::SetRate(double new_rate)
	{
		interest_rate_ = new_rate;
	}

	double Account::InitRate()
	{
		return 3.14;
	}

	// 必须定义并初始化静态数据成员
	double Account::interest_rate_ = InitRate();

	// 必须定义静态数据成员。如果在类的内部提供了一个初始值，则成员的定义不能再指定一个初始值了
	constexpr int Account::period_;
}
