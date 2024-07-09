#ifndef ALLOCATOR_UTILITY_H
#define ALLOCATOR_UTILITY_H

#include <cstdlib>
#include "Assert.h"

namespace Portal
{
	// Computes the memory alignment at compile time
	constexpr std::size_t AlignMemoryAddressForward(std::size_t marker,
		std::size_t alignment)
	{
		const std::size_t mask = alignment - 1;
		// We need to ensure that the alignment is a power of 2
		ASSERT((alignment & mask) == 0, "Alignment was not a power of 2 when "\
			"aligning memory address.");
		// Shifts the address forwards
		return (marker + mask) & ~mask;
	}
	// ------------------------------------------------------------------------
	// Computes the memory alignment at compile time
	constexpr std::size_t AlignMemoryAddressBackward(std::size_t marker,
		std::size_t alignment)
	{
		const std::size_t mask = alignment - 1;
		// We need to ensure that the alignment is a power of 2
		ASSERT((alignment & mask) == 0, "Alignment was not a power of 2 when "\
			"aligning memory address.");
		// Shifts the address backwards
		return marker & ~mask;
	}
} // namespace Portal

#endif