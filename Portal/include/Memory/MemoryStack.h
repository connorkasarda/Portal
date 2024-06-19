#ifndef MEMORY_STACK_H
#define MEMORY_STACK_H

#include <cstdlib>
#include <stack>

namespace Portal
{
	// One-sided stack memory allocator
	class MemoryStack final
	{
	public:
		// Constructor
		explicit MemoryStack(std::size_t size);
		// Destructor
		~MemoryStack();
		// Allocates a chunk in memory stack
		void* Allocate(std::size_t chunkSize);
		// Deallocates chunk in memory stack
		void Deallocate(void* ptr);
		// Reset allocator to original state
		void Reset();
	private:
		// Pointer to start of memory stack block
		void* memory{};
		// Size of memory stack block
		std::size_t size{};
		// Marks top of stack in memory block
		std::size_t marker{};
		// Record of chunk sizes in stack order
		std::stack<std::size_t> chunkSizes{};
	}; // class MemoryStack
} // namespace Portal

#endif // MEMORY_STACK_H