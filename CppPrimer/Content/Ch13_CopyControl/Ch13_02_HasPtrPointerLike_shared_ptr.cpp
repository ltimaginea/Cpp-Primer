#include <iostream>
#include <string>
#include <memory>

class HasPtr
{
public:
	HasPtr(const std::string& str = std::string()) :ps_(std::make_shared<std::string>(str)), i_(0) {  }
	HasPtr(const HasPtr& rhs) :ps_(rhs.ps_), i_(rhs.i_) {  }
	HasPtr& operator=(const HasPtr& rhs) { ps_ = rhs.ps_; i_ = rhs.i_; return *this; }
	~HasPtr() {  }

	HasPtr(HasPtr&& rhs) noexcept :ps_(std::move(rhs.ps_)), i_(rhs.i_) {  }
	HasPtr& operator=(HasPtr&& rhs) noexcept { ps_ = std::move(rhs.ps_); i_ = rhs.i_; return *this; }

private:
	std::shared_ptr<std::string> ps_;
	int i_;
};

int main()
{
	HasPtr hp1("Sorry, I am so tired.");
	HasPtr hp2(hp1);
	HasPtr hp3(hp2);

	HasPtr hp4("Just Courage!");
	hp4 = std::move(hp4);
	hp4 = std::move(hp3);
	HasPtr hp5("Focus on!");
	hp3 = hp5;

	return 0;
}
