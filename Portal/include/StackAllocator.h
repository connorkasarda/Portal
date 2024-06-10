#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <stack>
#include "Allocator.h"

namespace Portal
{
	// One-sided stack memory allocator
	class StackAllocator : public Allocator
	{
	public:
		// Constructor
		explicit StackAllocator(std::size_t size);
		// Destructor
		~StackAllocator() override;
		// Allocates a block in memory stack
		void* Allocate(std::size_t blockSize) override;
		// Deallocates block in memory stack
		void Deallocate(void* ptr) override;
		// Reset allocator to original state
		void Reset() override;
	private:
		// Pointer to allocated memory
		void* memory{};
		// Size of allocated memory
		std::size_t size{};
		// Marks current position in allocated memory
		std::size_t top{};
		// Record of block sizes in stack order
		std::stack<size_t> blockSizes{};
	}; // class StackAllocator
} // namespace Portal

#endif // STACK_ALLOCATOR_H