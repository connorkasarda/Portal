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
	void* BistackAllocator::AllocateFront(std::size_t chunkSize,
		std::size_t alignment)
	{
		std::size_t alignedFrontMarker = AlignMemoryAddressBackward(
			frontMarker - chunkSize, alignment);
		if (alignedFrontMarker < backMarker)
		{
			ASSERT(false, "Bilateral allocator ran out of memory during "\
				"front-end stack allocation");
			return nullptr;
		}
		// Order of shifting frontMarker and assigning pointer crucial! With
		// the AllocateFront method, where we allocate new chunk and where the
		// front marker is located are different. frontMarker must be shifted
		// towards the back-size stack first, then allocation to ptr follows.
		frontMarker = alignedFrontMarker;
		void* ptr = static_cast<char*>(memory) + frontMarker;
		frontChunkSizes.push(chunkSize);
		return ptr;
	}
	// ------------------------------------------------------------------------
	void* BistackAllocator::AllocateBack(std::size_t chunkSize,
		std::size_t alignment)
	{
		const std::size_t alignedBackMarker = AlignMemoryAddressForward(
			backMarker, alignment);
		if (backMarker + chunkSize > frontMarker)
		{
			ASSERT(false, "Bilateral allocator ran out of memory during "\
				"back-end stack allocation");
			return nullptr;
		}
		// Because backMarker and location to add new chunk are the same, we 
		// perform the ptr allocation first and then increment the backMarker.
		void* ptr = static_cast<char*>(memory) + backMarker;
		backMarker += chunkSize;
		backChunkSizes.push(chunkSize);
		return ptr;
	}
	// ------------------------------------------------------------------------
	void BistackAllocator::DeallocateFront(void* ptr)
	{
		if (frontChunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Bistack Memory "\
				"Front-side deallocation call.");
			return;
		}
		if (ptr != static_cast<char*>(memory) + frontMarker)
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
	void BistackAllocator::DeallocateBack(void* ptr)
	{
		if (backChunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Bistack Memory "\
				"Back-side deallocation call.");
			return;
		}
		size_t topChunkSize = backChunkSizes.top();
		if (ptr != static_cast<char*>(memory) + backMarker - topChunkSize)
		{
			ASSERT(false, "Invalid pointer passed to Bistack Memory "\
				"Back-side deallocation method. Incorrect position in stack.");
			return;
		}
		backMarker -= topChunkSize;
		backChunkSizes.pop();
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