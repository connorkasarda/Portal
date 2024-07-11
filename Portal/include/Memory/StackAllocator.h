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
		// Template function implementations need to be defined in the header
		template <typename T>
		T* Allocate();
		// Deallocates chunk in memory stack
		template <typename T>
		void Deallocate(T* chunkPtr);
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

	// Template Implementations
	template <typename T>
	T* StackAllocator::Allocate()
	{
		std::size_t chunkSize = sizeof(T);
		std::size_t alignedMarker = AlignMemoryAddressForward(marker,
			alignof(T));
		if (alignedMarker + chunkSize > size)
		{
			ASSERT(false, "Stack Allocator ran out of memory during "\
				"allocation.");
			return nullptr;
		}
		void* chunkPtr = static_cast<char*>(memory) + alignedMarker;
		marker = alignedMarker + chunkSize;
		chunkSizes.push(chunkSize);
		return static_cast<T*>(chunkPtr);
	}
	// ------------------------------------------------------------------------
	template <typename T>
	void StackAllocator::Deallocate(T* chunkPtr)
	{
		if (chunkSizes.empty())
		{
			ASSERT(false, "No allocated blocks during Stack Allocator "\
				"deallocation call.");
			return;
		}
		// This seems a little insane but is necessary ...
		// 1. We static cast memory to char* to allow arithmetic
		// 2. Now, we take arithmetic and create intermediate void*
		// 3. Produce correct T* for if check
		if (chunkPtr != static_cast<T*>(
			static_cast<void*>(
				static_cast<char*>(memory) + marker - chunkSizes.top())))
		{
			ASSERT(false, "Invalid pointer passed to Stack Allocator "\
				"deallocation method. Incorrect position in stack.");
			return;
		}
		marker -= chunkSizes.top();
		chunkSizes.pop();
	}
} // namespace Portal

#endif // STACK_ALLOCATOR_H