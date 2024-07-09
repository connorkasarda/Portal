#include "Assert.h"
#include "PoolAllocator.h"

namespace Portal
{
	PoolAllocator::PoolAllocator(std::size_t sizeOfChunks, std::size_t numChunks)
		: sizeOfChunks(sizeOfChunks)
		, numChunks(numChunks)
		, memory(nullptr)
	{
		memory = std::malloc(sizeOfChunks * numChunks);
		ASSERT(memory != nullptr, "Memory Pool could not allocate block of "\
			"memory during construction");
		for (size_t c = 0; c < numChunks; ++c)
		{
			void* chunk = 
				static_cast<char*>(memory) + (c * sizeOfChunks);
			chunks.push(chunk);
		}
	}
	// ------------------------------------------------------------------------
	PoolAllocator::~PoolAllocator()
	{
		std::free(memory);
		while (!chunks.empty()) chunks.pop();
	}
	// ------------------------------------------------------------------------
	void* PoolAllocator::Allocate()
	{
		if (chunks.empty())
		{
			ASSERT(false, "All chunks in Memory Pool are allocated!");
			return nullptr;
		}
		void* ptr = chunks.top();
		chunks.pop();
		return ptr;
	}
	// ------------------------------------------------------------------------
	void PoolAllocator::Deallocate(void* ptr)
	{
		chunks.push(ptr);
	}
	// ------------------------------------------------------------------------
	void PoolAllocator::Reset()
	{
		while (!chunks.empty()) chunks.pop();
		for (size_t c = 0; c < numChunks; ++c)
		{
			void* chunk =
				static_cast<char*>(memory) + (c * sizeOfChunks);
			chunks.push(chunk);
		}
	}
} // namespace Portal