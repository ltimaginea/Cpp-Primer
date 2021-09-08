#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <initializer_list>

class StrVec
{
public:
	StrVec() :elements_(nullptr), first_free_(nullptr), cap_(nullptr) {  }
	StrVec(const StrVec&);
	StrVec(StrVec&&) noexcept;
	StrVec(const std::initializer_list<std::string>&);
	StrVec& operator=(const StrVec&);
	StrVec& operator=(StrVec&&) noexcept;
	StrVec& operator=(const std::initializer_list<std::string>&);
	~StrVec();
	void push_back(const std::string&);
	void push_back(std::string&&) noexcept;
	size_t size() const { return first_free_ - elements_; }
	size_t capacity() const { return cap_ - elements_; }
	std::string* begin() const { return elements_; }
	std::string* end() const { return first_free_; }
	void reserve(size_t n);
	void resize(size_t n);
	void resize(size_t n, const std::string& val);
private:
	void free();
	void reallocate(bool is_specified = false, size_t new_cap = 1);
	std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
	void chk_n_alloc() { if (size() == capacity()) reallocate(); }
private:
	// StrVec's allocator should be a static member not an ordinary member
	static std::allocator<std::string> alloc_;
	std::string* elements_;
	std::string* first_free_;
	std::string* cap_;
};

void StrVec::push_back(const std::string& str)
{
	chk_n_alloc();
	alloc_.construct(first_free_++, str);
}

void StrVec::push_back(std::string&& str) noexcept
{
	chk_n_alloc();
	alloc_.construct(first_free_++, std::move(str));
}

std::pair<std::string*, std::string*> StrVec::alloc_n_copy(const std::string* b, const std::string* e)
{
	auto data = alloc_.allocate(e - b);
	return { data,std::uninitialized_copy(b,e,data) };
}

void StrVec::free()
{
	if (elements_)
	{
		for (auto p = first_free_; p != elements_;)
			alloc_.destroy(--p);
		alloc_.deallocate(elements_, cap_ - elements_);
	}
}

// 这里的reallocate实现，仅适合增大容量的情形，缩减容量的情形需要重新实现（如果是缩减，下面的实现的构造部分alloc_.construct将可能会因为内存越界而出错）
void StrVec::reallocate(bool is_specified, size_t new_cap)
{
	size_t new_capacity = 1;
	if (is_specified)
		new_capacity = new_cap;
	else
		new_capacity = capacity() ? 2 * capacity() : 1;
	auto new_data = alloc_.allocate(new_capacity);
	auto dest = new_data;
	auto elem = elements_;
	for (size_t i = 0; i != size(); ++i)
	{
		alloc_.construct(dest, std::move(*elem));
		++dest;
		++elem;
	}
	free();
	elements_ = new_data;
	first_free_ = dest;
	cap_ = new_data + new_capacity;
}

StrVec::StrVec(const StrVec& sv)
{
	auto data = alloc_n_copy(sv.begin(), sv.end());
	elements_ = data.first;
	first_free_ = data.second;
	cap_ = data.second;
}

StrVec::StrVec(StrVec&& sv) noexcept :elements_(sv.elements_), first_free_(sv.first_free_), cap_(sv.cap_)
{
	sv.elements_ = nullptr;
	sv.first_free_ = nullptr;
	sv.cap_ = nullptr;
}

StrVec::StrVec(const std::initializer_list<std::string>& il)
{
	auto data = alloc_n_copy(il.begin(), il.end());
	elements_ = data.first;
	first_free_ = data.second;
	cap_ = data.second;
}

StrVec& StrVec::operator=(const StrVec& sv)
{
	auto data = alloc_n_copy(sv.begin(), sv.end());
	free();
	elements_ = data.first;
	first_free_ = data.second;
	cap_ = data.second;
	return *this;
}

StrVec& StrVec::operator=(StrVec&& sv) noexcept
{
	// direct test for self-assignment
	if (this != &sv)
	{
		free();
		elements_ = sv.elements_;
		first_free_ = sv.first_free_;
		cap_ = sv.cap_;
		sv.elements_ = nullptr;
		sv.first_free_ = nullptr;
		sv.cap_ = nullptr;
	}
	return *this;
}

StrVec& StrVec::operator=(const std::initializer_list<std::string>& il)
{
	auto data = alloc_n_copy(il.begin(), il.end());
	free();
	elements_ = data.first;
	first_free_ = data.second;
	cap_ = data.second;
	return *this;
}

StrVec::~StrVec()
{
	free();
}

void StrVec::reserve(size_t n)
{
	if (n > capacity())
		reallocate(true, n);
}

void StrVec::resize(size_t n)
{
	resize(n, std::string());
}

void StrVec::resize(size_t n, const std::string& val)
{
	if (n > size())
	{
		for (size_t i = size(); i != n; ++i)
			push_back(val);
	}
	else if (n < size())
	{
		for (size_t i = size(); i != n; --i)
			alloc_.destroy(--first_free_);
	}
}

// definition for static data member
std::allocator<std::string> StrVec::alloc_;

int main()
{
	StrVec sv;
	sv.push_back("promise");
	sv.push_back("the beginning");
	sv.push_back("done");
	sv.push_back("world");
	sv.push_back("spring");
	sv.push_back("cpp primer");
	std::string str("1234567");
	sv.push_back(str);
	std::cout << sv.size() << " " << sv.capacity() << std::endl;
	std::cout << *sv.begin() << std::endl;
	sv.reserve(32);
	sv.resize(40);
	sv.resize(3);
	std::cout << sv.size() << " " << sv.capacity() << std::endl;

	StrVec sv2(sv);
	StrVec sv3(std::move(sv));
	StrVec sv4;
	sv4.push_back("thinking");
	StrVec sv5;
	sv5.push_back("experience");
	sv4 = std::move(sv5);
	sv4 = sv2;

	StrVec sv6 = { "one", "two", "three" };
	sv6 = { "first","second","third" };

	return 0;
}
