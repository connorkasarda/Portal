#include "Debug.h"
#include "MemoryBistack.h"

namespace Portal
{
	MemoryBistack::MemoryBistack(std::size_t size)
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
	MemoryBistack::~MemoryBistack()
	{
		std::free(memory);
	}
	// ------------------------------------------------------------------------
	void* MemoryBistack::AllocateFront(std::size_t chunkSize)
	{
		if (frontMarker - chunkSize < backMarker)
		{
			ASSERT(false, "Bilateral allocator ran out of memory during "\
				"lower-end stack allocation");
			return nullptr;
		}
		void* ptr = static_cast<char*>(memory) + frontMarker;
		frontMarker -= chunkSize;
		frontChunkSizes.push(chunkSize);
		return ptr;
	}
	// ------------------------------------------------------------------------
	void* MemoryBistack::AllocateBack(std::size_t chunkSize)
	{
		if (backMarker + chunkSize > frontMarker)
		{
			ASSERT(false, "Bilateral allocator ran out of memory during "\
				"upper-end stack allocation");
			return nullptr;
		}
		void* ptr = static_cast<char*>(memory) + backMarker;
		backMarker += chunkSize;
		backChunkSizes.push(chunkSize);
		return ptr;
	}
	// ------------------------------------------------------------------------
	void MemoryBistack::DeallocateFront(void* ptr)
	{
		if (frontChunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Bistack Memory "\
				"Front-side deallocation call.");
			return;
		}
		if (ptr != static_cast<char*>(memory) + frontMarker + 
			frontChunkSizes.top())
		{
			ASSERT(false, "Invalid pointer passed to Bistack Memory "\
				"Front-side deallocation method. Incorrect position in "\
				"stack.");
			return;
		}
		frontMarker += frontChunkSizes.top();
		frontChunkSizes.pop();
	}
	// ------------------------------------------------------------------------
	void MemoryBistack::DeallocateBack(void* ptr)
	{
		if (backChunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Bistack Memory "\
				"Back-side deallocation call.");
			return;
		}
		if (ptr != static_cast<char*>(memory) + backMarker -
			backChunkSizes.top())
		{
			ASSERT(false, "Invalid pointer passed to Bistack Memory "\
				"Back-side deallocation method. Incorrect position in stack.");
			return;
		}
		backMarker -= backChunkSizes.top();
		backChunkSizes.pop();
	}
	// ------------------------------------------------------------------------
	void MemoryBistack::Reset()
	{
		backMarker = 0;
		frontMarker = size;
		while (!backChunkSizes.empty()) backChunkSizes.pop();
		while (!frontChunkSizes.empty()) frontChunkSizes.pop();
	}
} // namespace Portal