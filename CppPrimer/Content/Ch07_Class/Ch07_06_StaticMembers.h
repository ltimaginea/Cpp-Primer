#ifndef CPPPRIMER_CONTENT_CH07_CLASS_CH07_06_STATICMEMBERS_H_
#define CPPPRIMER_CONTENT_CH07_CLASS_CH07_06_STATICMEMBERS_H_

#include <string>

namespace static_members
{
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
} // namespace static_members

#endif // !CPPPRIMER_CONTENT_CH07_CLASS_CH07_06_STATICMEMBERS_H_
