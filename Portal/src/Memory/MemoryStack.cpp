#include "Debug.h"
#include "MemoryStack.h"

namespace Portal
{
	MemoryStack::MemoryStack(std::size_t size)
		: memory(nullptr)
		, size(size)
		, marker(0)
	{
		memory = std::malloc(size);
		ASSERT(memory != nullptr, "Failed to acquire memory for stack "\
			"allocator when constructed");
	}
	// ------------------------------------------------------------------------
	MemoryStack::~MemoryStack()
	{
		std::free(memory);
	}
	// ------------------------------------------------------------------------
	void* MemoryStack::Allocate(std::size_t chunkSize)
	{
		if (marker + chunkSize > size)
		{
			ASSERT(false, "Stack Memory ran out of memory during "\
				"allocation.");
			return nullptr;
		}
		void* ptr = static_cast<char*>(memory) + marker;
		marker += chunkSize;
		chunkSizes.push(chunkSize);
		return ptr;
	}
	// ------------------------------------------------------------------------
	void MemoryStack::Deallocate(void* ptr)
	{
		if (chunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Stack Memory "\
				"deallocation call.");
			return;
		}
		if (ptr != static_cast<char*>(memory) + marker - chunkSizes.top())
		{
			ASSERT(false, "Invalid pointer passed to Stack Memory "\
				"deallocation method. Incorrect position in stack.");
			return;
		}
		marker -= chunkSizes.top();
		chunkSizes.pop();
	}
	// ------------------------------------------------------------------------
	void MemoryStack::Reset()
	{
		marker = 0;
		while (!chunkSizes.empty()) chunkSizes.pop();
	}
} // namespace Portal