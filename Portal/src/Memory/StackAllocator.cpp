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
	void* StackAllocator::Allocate(std::size_t chunkSize,
		std::size_t alignment)
	{
		std::size_t alignedMarker = AlignMemoryAddressForward(marker,
			alignment);
		if (alignedMarker + chunkSize > size)
		{
			ASSERT(false, "Stack Allocator ran out of memory during "\
				"allocation.");
			return nullptr;
		}
		void* ptr = static_cast<char*>(memory) + alignedMarker;
		marker = alignedMarker + chunkSize;
		chunkSizes.push(chunkSize);
		return ptr;
	}
	// ------------------------------------------------------------------------
	void StackAllocator::Deallocate(void* ptr)
	{
		if (chunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Stack Allocator "\
				"deallocation call.");
			return;
		}
		if (ptr != static_cast<char*>(memory) + marker - chunkSizes.top())
		{
			ASSERT(false, "Invalid pointer passed to Stack Allocator "\
				"deallocation method. Incorrect position in stack.");
			return;
		}
		marker -= chunkSizes.top();
		chunkSizes.pop();
	}
	// ------------------------------------------------------------------------
	void StackAllocator::Reset()
	{
		marker = 0;
		while (!chunkSizes.empty()) chunkSizes.pop();
	}
} // namespace Portal