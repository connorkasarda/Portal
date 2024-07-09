#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <cstdlib>
#include <stack>

namespace Portal
{
	// One-sided stack memory allocator
	class StackAllocator final
	{
	public:
		// Constructor
		explicit StackAllocator(std::size_t size);
		// Destructor
		~StackAllocator();
		// Allocates a chunk in memory stack
		void* Allocate(std::size_t chunkSize, std::size_t alignment);
		// Deallocates chunk in memory stack
		void Deallocate(void* ptr);
		// Reset allocator to original state
		void Reset();
	private:
		// Pointer to start of memory stack block
		void* memory;
		// Size of memory stack block
		std::size_t size;
		// Marks top of stack in memory block
		std::size_t marker;
		// Record of chunk sizes in stack order
		std::stack<std::size_t> chunkSizes; 
	}; // class StackAllocator
} // namespace Portal

#endif // STACK_ALLOCATOR_H