// rvalue-references-move-semantics.cpp
#include <iostream>
#include <algorithm>
#include <vector>

class MemoryBlock
{
public:
	// Simple constructor that initializes the resource.
	explicit MemoryBlock(size_t length) :_data(length) {  }

	// Destructor.
	~MemoryBlock() = default;

	// Copy constructor.
	MemoryBlock(const MemoryBlock& other) = default;

	// Copy assignment operator.
	MemoryBlock& operator=(const MemoryBlock& other) = default;

	// Move constructor.
	MemoryBlock(MemoryBlock&& other) = default;

	// Move assignment operator.
	MemoryBlock& operator=(MemoryBlock&& other) = default;

	// Retrieves the length of the data resource.
	size_t Length() const { return _data.size(); }
private:
	std::vector<int> _data;
};

int main()
{
	// Create a vector object and add a few elements to it.
	std::vector<MemoryBlock> v;
	v.push_back(MemoryBlock(25));
	v.push_back(MemoryBlock(75));
	auto m1 = MemoryBlock(95);
	v.push_back(m1);
	// Insert a new element into the second position of the vector.
	v.insert(v.begin() + 1, MemoryBlock(50));
	auto m2 = MemoryBlock(955);
	m1 = m2;
	m1 = MemoryBlock(965);
	return 0;
}
