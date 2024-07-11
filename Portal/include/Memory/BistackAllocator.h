#ifndef BISTACK_ALLOCATOR_H
#define BISTACK_ALLOCATOR_H

#include <cstdlib>
#include <stack>

namespace Portal
{
	// Two-sided stack memory allocator
	class BistackAllocator final
	{
	public:
		// Constructor
		explicit BistackAllocator(std::size_t size);
		// Destructor
		~BistackAllocator();
		// Allocates to the front-side of the memory
		template <typename T>
		T* AllocateFront();
		// Allocates to the back-side of the memory
		template <typename T>
		T* AllocateBack();
		// Deallocates from top of front-side stack
		template <typename T>
		void DeallocateFront(T* chunkPtr);
		// Deallocates from top of back-side stack
		template <typename T>
		void DeallocateBack(T* chunkPtr);
		// Resets allocator to zero state
		void Reset();
	private:
		// Pointer to front-side of memory bistack block
		void* memory;
		// Size of the memory bistack block
		std::size_t size;
		// Marks top of the front-side stack
		std::size_t frontMarker;
		// Marks top of the back-side stack
		std::size_t backMarker;
		// Stack of chunk sizes in front stack
		std::stack<std::size_t> frontChunkSizes;
		// Stack of chunk sizes in back stack
		std::stack<std::size_t> backChunkSizes;
	}; // class BistackAllocator

	// Template implementations
	template <typename T>
	T* BistackAllocator::AllocateFront()
	{
		std::size_t chunkSize = sizeof(T);
		std::size_t alignedFrontMarker = AlignMemoryAddressBackward(
			frontMarker - chunkSize, alignof(T));
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
		void* chunkPtr = static_cast<char*>(memory) + frontMarker;
		frontChunkSizes.push(chunkSize);
		return static_cast<T*>(chunkPtr);
	}
	// ------------------------------------------------------------------------
	template <typename T>
	T* BistackAllocator::AllocateBack()
	{
		std::size_t chunkSize = sizeof(T);
		const std::size_t alignedBackMarker = AlignMemoryAddressForward(
			backMarker, alignof(T));
		if (backMarker + chunkSize > frontMarker)
		{
			ASSERT(false, "Bilateral allocator ran out of memory during "\
				"back-end stack allocation");
			return nullptr;
		}
		// Because backMarker and location to add new chunk are the same, we 
		// perform the ptr allocation first and then increment the backMarker.
		void* chunkPtr = static_cast<char*>(memory) + backMarker;
		backMarker += chunkSize;
		backChunkSizes.push(chunkSize);
		return static_cast<T*>(chunkPtr);
	}
	// ------------------------------------------------------------------------
	template <typename T>
	void BistackAllocator::DeallocateFront(T* chunkPtr)
	{
		if (frontChunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Bistack Memory "\
				"Front-side deallocation call.");
			return;
		}
		if (chunkPtr != static_cast<T*>(
			static_cast<void*>(static_cast<char*>(memory) + frontMarker)))
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
	template <typename T>
	void BistackAllocator::DeallocateBack(T* chunkPtr)
	{
		if (backChunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Bistack Memory "\
				"Back-side deallocation call.");
			return;
		}
		size_t topChunkSize = backChunkSizes.top();
		if (chunkPtr != static_cast<T*>(
			static_cast<void*>(
				static_cast<char*>(memory) + backMarker - topChunkSize)))
		{
			ASSERT(false, "Invalid pointer passed to Bistack Memory "\
				"Back-side deallocation method. Incorrect position in stack.");
			return;
		}
		backMarker -= topChunkSize;
		backChunkSizes.pop();
	}
} // namespace Portal

#endif // BISTACK_ALLOCATOR_H