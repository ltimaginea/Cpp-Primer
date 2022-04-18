#ifndef CPPPRIMER_CONTENT_CH13_COPYCONTROL_CH13_00_PIMPL_CH13_00_PIMPL_WIDGET_H_
#define CPPPRIMER_CONTENT_CH13_COPYCONTROL_CH13_00_PIMPL_CH13_00_PIMPL_WIDGET_H_

#include <memory>

class Widget
{
public:
	// even the default constructor needs to be defined in the implementation file
	Widget();
	explicit Widget(int);
	Widget(const Widget&);
	Widget(Widget&&);
	Widget& operator=(const Widget&);
	Widget& operator=(Widget&&);
	~Widget();

	// public API that will be forwarded to the implementation
	void Draw();
	void Draw() const;
	void PrintInfo() const;
	// public API that implementation has to call
	bool Shown() const { return true; }
	// ...

private:
	// things to be hidden go here
	class Impl;
	// opaque pointer to forward-declared class
	std::unique_ptr<Impl> pimpl_;
};

#endif // !CPPPRIMER_CONTENT_CH13_COPYCONTROL_CH13_00_PIMPL_CH13_00_PIMPL_WIDGET_H_
