#include <iostream>
#include <memory>

class VFuncBase
{
public:
	VFuncBase() = default;
	VFuncBase(const VFuncBase&) = default;
	VFuncBase(VFuncBase&&) = default;
	VFuncBase& operator=(const VFuncBase&) = default;
	VFuncBase& operator=(VFuncBase&&) = default;
	virtual ~VFuncBase() = default;
public:
	virtual int GetState() { return state_; }
protected:
	int state_ = 0;
};

class VFuncDerived : public VFuncBase
{
public:
	VFuncDerived() = default;
	VFuncDerived(const VFuncDerived&) = default;
	VFuncDerived(VFuncDerived&&) = default;
	VFuncDerived& operator=(const VFuncDerived&) = default;
	VFuncDerived& operator=(VFuncDerived&&) = default;
	~VFuncDerived() override = default;
private:
	int GetState() override { return state_; }
};

int main()
{
	int state{ 0 };
	VFuncDerived vfd;			// Object of derived type.
	VFuncBase& rvfb = vfd;		// Reference to base type.
	VFuncDerived& rvfd = vfd;	// Reference to derived type.

	state = rvfb.GetState();	// GetState is public.
	//state = rvfd.GetState();	// error! GetState is private.

	std::unique_ptr<VFuncBase> ptr_base = std::make_unique<VFuncDerived>();
	std::unique_ptr<VFuncDerived> ptr_derived = std::make_unique<VFuncDerived>();

	state = ptr_base->GetState();		// OK
	//state = ptr_derived->GetState();	// error! GetState is private.

	return 0;
}

// Tips:
//	1. Access rules for the names of virtual functions are checked at the call point using the type of the expression used to denote the object for which the member function is called. 
//	   The access of the final overrider is ignored.
//	2. 一般情况下，基类与派生类的同名同参数表的虚函数的访问范围说明符应保持一致。
