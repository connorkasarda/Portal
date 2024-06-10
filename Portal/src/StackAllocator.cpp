#include "Debug.h"
#include "StackAllocator.h"

namespace Portal
{
	StackAllocator::StackAllocator(std::size_t size)
		: memory(std::malloc(size))
		, size(size)
		, top(0)
	{
	}
	// ------------------------------------------------------------------------
	StackAllocator::~StackAllocator()
	{
		std::free(memory);
	}
	// ------------------------------------------------------------------------
	void* StackAllocator::Allocate(std::size_t blockSize)
	{
		if (top + blockSize > size)
		{
			ASSERT(false, "Stack Allocator ran out of memory during "\
				"allocation.");
			return nullptr;
		}
		void* ptr = static_cast<char*>(memory) + top;
		top += blockSize;
		blockSizes.push(blockSize);
		return ptr;
	}
	// ------------------------------------------------------------------------
	void StackAllocator::Deallocate(void* ptr)
	{
		if (blockSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Stack Allocator "\
				"deallocation call.");
			return;
		}
		if (ptr != static_cast<char*>(memory) + top - blockSizes.top())
		{
			ASSERT(false, "Invalid pointer passed to Stack Allocator "\
				"deallocation method.");
			return;
		}
		top -= blockSizes.top();
		blockSizes.pop();
	}
	// ------------------------------------------------------------------------
	void StackAllocator::Reset()
	{
		top = 0;
		while (!blockSizes.empty()) blockSizes.pop();
	}
} // namespace Portal