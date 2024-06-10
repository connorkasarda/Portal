#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdlib>

namespace Portal
{
	// Base class for memory allocators
	class Allocator
	{
	public:
		// Abstract destructor
		virtual ~Allocator() = default;
		// Allocate memory
		virtual void* Allocate(std::size_t blockSize) = 0;
		// Deallocate memory
		virtual void Deallocate(void* ptr) = 0;
		// Reset allocator to original state
		virtual void Reset() = 0;
	}; // class Allocator
} // namespace Portal

#endif // ALLOCATOR_H