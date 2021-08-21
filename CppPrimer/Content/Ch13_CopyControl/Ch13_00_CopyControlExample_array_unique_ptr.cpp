// rvalue-references-move-semantics.cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>

class MemoryBlock
{
public:
	// Simple constructor that initializes the resource.
	explicit MemoryBlock(size_t length)
		: _length(length)
		, _data(std::make_unique<int[]>(length))
	{
		std::cout << "In MemoryBlock(size_t). length = "
			<< _length << "." << std::endl;
	}

	// Destructor.
	~MemoryBlock()
	{
		std::cout << "In ~MemoryBlock(). length = "
			<< _length << ".";
		std::cout << " Deleting resource." << std::endl;
	}

	// Copy constructor.
	MemoryBlock(const MemoryBlock& other)
		: _length(other._length)
		, _data(std::make_unique<int[]>(other._length))
	{
		std::cout << "In MemoryBlock(const MemoryBlock&). length = "
			<< other._length << ". Copying resource." << std::endl;

		std::copy(&other._data[0], &other._data[0] + other._length, &_data[0]);
	}

	// Copy assignment operator.
	MemoryBlock& operator=(const MemoryBlock& other)
	{
		std::cout << "In operator=(const MemoryBlock&). length = "
			<< other._length << ". Copying resource." << std::endl;

		//if (this != &other)	// 不必再进行“自赋值”判断，下面的写法可以处理“自赋值”的情况，同时也是“异常安全”的
		{
			// 因为左侧和右侧对象的数组大小可能不同，所以无法直接将右侧对象的资源内容拷贝至左侧对象，这也是以制造临时对象来完成拷贝的另一目的
			auto newp = std::make_unique<int[]>(other._length);
			std::copy(&other._data[0], &other._data[0] + other._length, &newp[0]);

			_length = other._length;
			_data = std::move(newp);

		}
		return *this;
	}

	// Move constructor.
	MemoryBlock(MemoryBlock&& other) noexcept
		: _length(other._length)
		, _data(std::move(other._data))
	{
		std::cout << "In MemoryBlock(MemoryBlock&&). length = "
			<< other._length << ". Moving resource." << std::endl;

		other._length = 0;
		//other._data = nullptr;	// 不需要再显式置为nullptr，移动赋值会使右值置为nullptr
	}

	// Move assignment operator.
	MemoryBlock& operator=(MemoryBlock&& other) noexcept
	{
		std::cout << "In operator=(MemoryBlock&&). length = "
			<< other._length << "." << std::endl;

		//if (this != &other)		// 不用再进行自赋值判断，unique_ptr的operator=可以处理自赋值的情况
		{
			_length = other._length;
			_data = std::move(other._data);

			other._length = 0;
			//other._data = nullptr;	// 不需要再显式置为nullptr，移动赋值会使右值置为nullptr
		}
		return *this;
	}

	// Retrieves the length of the data resource.
	size_t Length() const
	{
		return _length;
	}

private:
	size_t _length; // The length of the resource.
	std::unique_ptr<int[]> _data;	// The resource.
};

int main()
{
	// Create a vector object and add a few elements to it.
	std::vector<MemoryBlock> v;
	v.push_back(MemoryBlock(25));
	v.push_back(MemoryBlock(75));

	// Insert a new element into the second position of the vector.
	v.insert(v.begin() + 1, MemoryBlock(50));

	return 0;
}

/* Outputs:
In MemoryBlock(size_t). length = 25.
In MemoryBlock(MemoryBlock&&). length = 25. Moving resource.
In ~MemoryBlock(). length = 0. Deleting resource.
In MemoryBlock(size_t). length = 75.
In MemoryBlock(MemoryBlock&&). length = 75. Moving resource.
In MemoryBlock(MemoryBlock&&). length = 25. Moving resource.
In ~MemoryBlock(). length = 0. Deleting resource.
In ~MemoryBlock(). length = 0. Deleting resource.
In MemoryBlock(size_t). length = 50.
In MemoryBlock(MemoryBlock&&). length = 50. Moving resource.
In MemoryBlock(MemoryBlock&&). length = 25. Moving resource.
In MemoryBlock(MemoryBlock&&). length = 75. Moving resource.
In ~MemoryBlock(). length = 0. Deleting resource.
In ~MemoryBlock(). length = 0. Deleting resource.
In ~MemoryBlock(). length = 0. Deleting resource.
In ~MemoryBlock(). length = 25. Deleting resource.
In ~MemoryBlock(). length = 50. Deleting resource.
In ~MemoryBlock(). length = 75. Deleting resource.
*/
