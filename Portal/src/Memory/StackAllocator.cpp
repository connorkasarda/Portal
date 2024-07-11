#include "Assert.h"
#include "AllocatorUtility.h"
#include "StackAllocator.h"

namespace Portal
{
	StackAllocator::StackAllocator(std::size_t size)
		: memory(nullptr)
		, size(size)
		, marker(0)
		, chunkSizes()
	{
		memory = std::malloc(size);
		ASSERT(memory != nullptr, "Failed to acquire memory for Stack "\
			"Allocator when constructed");
	}
	// ------------------------------------------------------------------------
	StackAllocator::~StackAllocator()
	{
		std::free(memory);
	}
	// ------------------------------------------------------------------------
	void StackAllocator::Reset()
	{
		marker = 0;
		while (!chunkSizes.empty()) chunkSizes.pop();
	}
} // namespace Portal