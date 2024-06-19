#ifndef MEMORY_BISTACK_H
#define MEMORY_BISTACK_H

#include <cstdlib>
#include <stack>

namespace Portal
{
	// Two-sided stack memory allocator
	class MemoryBistack final
	{
	public:
		// Constructor
		explicit MemoryBistack(std::size_t size);
		// Destructor
		~MemoryBistack();
		// Allocates to the front-side of the memory
		void* AllocateFront(std::size_t chunkSize);
		// Allocates to the back-side of the memory
		void* AllocateBack(std::size_t chunkSize);
		// Deallocates from top of front-side stack
		void DeallocateFront(void* ptr);
		// Deallocates from top of back-side stack
		void DeallocateBack(void* ptr);
		// Resets allocator to zero state
		void Reset();
	private:
		// Pointer to front-side of memory bistack block
		void* memory{};
		// Size of the memory bistack block
		std::size_t size{};
		// Marks top of the front-side stack
		std::size_t frontMarker{};
		// Marks top of the back-side stack
		std::size_t backMarker{};
		// Stack of chunk sizes in front stack
		std::stack<std::size_t> frontChunkSizes{};
		// Stack of chunk sizes in back stack
		std::stack<std::size_t> backChunkSizes{};
	}; // class MemoryBistack
} // namespace Portal

#endif // MEMORY_BISTACK_H