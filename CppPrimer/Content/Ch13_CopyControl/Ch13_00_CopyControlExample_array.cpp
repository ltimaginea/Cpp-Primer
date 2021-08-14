// rvalue-references-move-semantics.cpp
#include <iostream>
#include <algorithm>
#include <vector>

class MemoryBlock
{
public:
	// Simple constructor that initializes the resource.
	explicit MemoryBlock(size_t length)
		: _length(length)
		, _data(new int[length])
	{
		std::cout << "In MemoryBlock(size_t). length = "
			<< _length << "." << std::endl;
	}

	// Destructor.
	~MemoryBlock()
	{
		std::cout << "In ~MemoryBlock(). length = "
			<< _length << ".";
		std::cout << " Deleting resource.";
		// Delete the resource.

		delete[] _data;

		std::cout << std::endl;
	}

	// Copy constructor.
	MemoryBlock(const MemoryBlock& other)
		: _length(other._length)
		, _data(new int[other._length])
	{
		std::cout << "In MemoryBlock(const MemoryBlock&). length = "
			<< other._length << ". Copying resource." << std::endl;

		std::copy(other._data, other._data + other._length, _data);
	}

	// Copy assignment operator.
	MemoryBlock& operator=(const MemoryBlock& other)
	{
		std::cout << "In operator=(const MemoryBlock&). length = "
			<< other._length << ". Copying resource." << std::endl;

		//if (this != &other)	// 不必再进行“自赋值”判断，下面的写法可以处理“自赋值”的情况，同时也是“异常安全”的
		{
			auto newp = new int[other._length];
			std::copy(other._data, other._data + other._length, newp);

			// Free the existing resource.
			delete[] _data;

			_length = other._length;
			_data = newp;

		}
		return *this;
	}

	// Move constructor.
	MemoryBlock(MemoryBlock&& other) noexcept
		: _length(other._length)
		, _data(other._data)
	{
		std::cout << "In MemoryBlock(MemoryBlock&&). length = "
			<< other._length << ". Moving resource." << std::endl;

		// Release the data pointer from the source object so that
		// the destructor does not free the memory multiple times.
		other._length = 0;
		other._data = nullptr;
	}

	// Move assignment operator.
	MemoryBlock& operator=(MemoryBlock&& other) noexcept
	{
		std::cout << "In operator=(MemoryBlock&&). length = "
			<< other._length << "." << std::endl;

		if (this != &other)
		{
			// Free the existing resource.
			delete[] _data;

			// Copy the data pointer and its length from the
			// source object.
			_length = other._length;
			_data = other._data;

			// Release the data pointer from the source object so that
			// the destructor does not free the memory multiple times.
			other._length = 0;
			other._data = nullptr;
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
	int* _data; // The resource.
};

int main()
{
	using namespace std;

	// Create a vector object and add a few elements to it.
	vector<MemoryBlock> v;
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
