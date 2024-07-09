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
		void* AllocateFront(std::size_t chunkSize, std::size_t alignment);
		// Allocates to the back-side of the memory
		void* AllocateBack(std::size_t chunkSize, std::size_t alignment);
		// Deallocates from top of front-side stack
		void DeallocateFront(void* ptr);
		// Deallocates from top of back-side stack
		void DeallocateBack(void* ptr);
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
} // namespace Portal

#endif // BISTACK_ALLOCATOR_H