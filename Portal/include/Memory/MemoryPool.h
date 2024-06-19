#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstdlib>
#include <stack>

namespace Portal
{
	// Preallocates memory whose size is a multiple of the size of the elements
	class MemoryPool final
	{
	public:
		// Constructor
		explicit MemoryPool(std::size_t sizeOfChunks, std::size_t numChunks);
		// Destructor
		~MemoryPool();
		// Returns pointer to allocated chunk
		void* Allocate();
		// Returns chunk to the pool
		void Deallocate(void* ptr);
	private:
		// The size of the chunks
		size_t sizeOfChunks{};
		// The number of chunks in the pool
		size_t numChunks{};
		// Pointer to beginning of memory pool block
		void* memory{};
		// Keeps track of pointers to chunks
		std::stack<void*> chunks{};
	}; // class MemoryPool
} // namespace Portal

#endif // MEMORY_POOL_H