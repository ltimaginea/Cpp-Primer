#include "Ch07_06_StaticMembers.h"

#include <iostream>
#include <memory>

int main()
{
	using static_members::Account;

	Account ac;
	auto ac_ptr = std::make_unique<Account>();
	double r = Account::GetRate();
	r = ac.GetRate();
	r = ac_ptr->GetRate();
	std::cout << sizeof(Account) << std::endl;

	return 0;
}
