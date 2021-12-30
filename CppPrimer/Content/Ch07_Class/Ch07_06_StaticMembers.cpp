#include <iostream>
#include <string>
#include <memory>

class Account
{
public:
	void Calculate() { amount_ += amount_ * interest_rate_; }
	static double GetRate() { return interest_rate_; }
	static void SetRate(double);
private:
	static double InitRate();
	std::string owner_;
	double amount_;
	static double interest_rate_;
	static constexpr int period_ = 30;
	double daily_tbl[period_];
};

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

int main()
{
	Account ac;
	auto ac_ptr = std::make_unique<Account>();
	double r = Account::GetRate();
	r = ac.GetRate();
	r = ac_ptr->GetRate();
	std::cout << sizeof(Account) << std::endl;

	return 0;
}
