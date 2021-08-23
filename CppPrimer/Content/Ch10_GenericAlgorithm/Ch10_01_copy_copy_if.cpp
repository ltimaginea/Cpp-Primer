// copy algorithm example
#include <iostream>
#include <algorithm>    // std::copy, std::copy_if
#include <vector>
#include <iterator>		// std::back_inserter

int main()
{
	int a1[] = { 0,1,2,3,4,5,6,7,8,9 };
	int a2[sizeof(a1) / sizeof(a1[0])];
	// copy的返回值是指向目标范围中最后复制元素的下个元素的输出迭代器
	std::copy(std::begin(a1), std::end(a1), a2);     // 把a1的内容拷贝给a2

	int myints[] = { 10,20,30,40,50,60,70 };
	std::vector<int> myvector(sizeof(myints) / sizeof(myints[0]));
	std::copy(std::begin(myints), std::end(myints), myvector.begin());

	std::vector<int> to_vector;
	// std::copy is equivalent to
	to_vector = myvector;

	std::vector<int> foo = { 252,152,52,-52,-152,852 };
	std::vector<int> bar;
	std::copy_if(foo.begin(), foo.end(), std::back_inserter(bar), [](int i) { return !(i < 0); });

	// std::back_inserter 之外的另一种方法，虽然效率不是很高
	std::vector<int> bar2(foo.size());
	// copy_if的返回值ret是指向目标范围中最后复制元素的下个元素的输出迭代器
	auto ret = std::copy_if(foo.begin(), foo.end(), bar2.begin(), [](int i) { return !(i < 0); });
	bar2.erase(ret, bar2.end());
	// or, alternatively,
	//bar2.resize(std::distance(bar2.begin(), ret));  // shrink container to new size

	return 0;
}

// Tips:
//  1. 数组指针也是迭代器
//	2. std::copy, std::copy_if 的返回值是指向目标范围中最后复制元素的下个元素的输出迭代器
//	3. 和memcpy相比，std::copy应该不存在大端小端问题
//	4. http://www.cplusplus.com/forum/general/103106/
