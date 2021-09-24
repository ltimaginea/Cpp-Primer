# Move Semantics



## 移动右值，拷贝左值



```cpp
#include <iostream>
#include <string>
#include <utility>

class OverloadRule
{
public:
	OverloadRule(std::string str = std::string()) :str_(str) {  }
	OverloadRule(const OverloadRule&) = default;
	OverloadRule(OverloadRule&&) = default;
	OverloadRule& operator=(const OverloadRule&) = default;
	OverloadRule& operator=(OverloadRule&&) = default;
	~OverloadRule() = default;
private:
	std::string str_;
};

int main()
{
	OverloadRule or1("courage"), or2("confidence");
	OverloadRule or3(or1);
	OverloadRule or4(std::move(or2));

	OverloadRule or5("spring"), or6("summer"), or7("autumn"), or8("winter");
	or5 = or6;
	or7 = std::move(or8);

	return 0;
}
```





## 如果没有移动构造函数，右值也被拷贝





```cpp
#include <iostream>
#include <string>
#include <utility>

class OverloadRule
{
public:
	OverloadRule(std::string str = std::string()) :str_(str) {  }
	OverloadRule(const OverloadRule&) = default;
	//OverloadRule(OverloadRule&&) = default;
	OverloadRule& operator=(const OverloadRule&) = default;
	//OverloadRule& operator=(OverloadRule&&) = default;
	~OverloadRule() = default;
private:
	std::string str_;
};

int main()
{
	OverloadRule or1("courage"), or2("confidence");
	OverloadRule or3(or1);
	OverloadRule or4(std::move(or2));

	OverloadRule or5("spring"), or6("summer"), or7("autumn"), or8("winter");
	or5 = or6;
	or7 = std::move(or8);

	return 0;
}
```

