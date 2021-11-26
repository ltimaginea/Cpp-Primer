#include <iostream>

class Base
{
public:
	virtual ~Base() = default;

	int GetState() { return state_; }
protected:
	void SetState(int state) { state_ = state; }
private:
	int state_ = 0;
};

class Derived : private Base	// 注意是private继承
{
public:
	~Derived() override = default;

	using Base::GetState;
protected:
	using Base::SetState;
	//using Base::state_;		// error! （无法访问）

	int GetValue() { return value_; }
private:
	int value_ = 1;
};

class Derived2 : private Derived	// 注意是private继承
{
public:
	~Derived2() override = default;

	using Derived::GetValue;
	using Derived::GetState;
	using Derived::SetState;

	//using Derived::state_;	// error! （无法访问）
	//using Derived::value_;	// error! （无法访问）

	//using Base::GetState;		// error! （经过测试发现，不能使用间接基类进行标记，只能使用直接基类进行标记）
};

int main()
{
	Base b;
	b.GetState();
	Derived d;
	d.GetState();
	Derived2 d2;
	d2.GetValue();
	d2.GetState();
	d2.SetState(2);

	return 0;
}
