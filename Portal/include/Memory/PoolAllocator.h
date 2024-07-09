#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <cstdlib>
#include <stack>

namespace Portal
{
	// Preallocates memory whose size is a multiple of the size of the elements
	class PoolAllocator final
	{
	public:
		// Constructor
		explicit PoolAllocator(std::size_t sizeOfChunks, std::size_t numChunks);
		// Destructor
		~PoolAllocator();
		// Returns pointer to allocated chunk
		void* Allocate();
		// Returns chunk to the pool
		void Deallocate(void* ptr);
		// Resets entire pool of memory
		void Reset();
	private:
		// The size of the chunks
		size_t sizeOfChunks;
		// The number of chunks in the pool
		size_t numChunks;
		// Pointer to beginning of memory pool block
		void* memory;
		// Keeps track of pointers to chunks
		std::stack<void*> chunks;
	}; // class PoolAllocator
} // namespace Portal

#endif // POOL_ALLOCATOR_H