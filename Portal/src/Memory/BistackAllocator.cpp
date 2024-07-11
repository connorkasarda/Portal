#include "Assert.h"
#include "AllocatorUtility.h"
#include "BistackAllocator.h"

namespace Portal
{
	BistackAllocator::BistackAllocator(std::size_t size)
		: memory(nullptr)
		, size(size)
		, backMarker(0)
		, frontMarker(size)
	{
		memory = std::malloc(size);
		ASSERT(memory != nullptr, "Failed to acquire memory for bilateral "\
			"allocator when constructed");
	}
	// ------------------------------------------------------------------------
	BistackAllocator::~BistackAllocator()
	{
		std::free(memory);
	}
	// ------------------------------------------------------------------------
	void BistackAllocator::Reset()
	{
		backMarker = 0;
		frontMarker = size;
		while (!backChunkSizes.empty()) backChunkSizes.pop();
		while (!frontChunkSizes.empty()) frontChunkSizes.pop();
	}
} // namespace Portal