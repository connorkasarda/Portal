#include "Debug.h"
#include "MemoryPool.h"

namespace Portal
{
	MemoryPool::MemoryPool(std::size_t sizeOfChunks, std::size_t numChunks)
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
	MemoryPool::~MemoryPool()
	{
		std::free(memory);
		while (!chunks.empty()) chunks.pop();
	}
	// ------------------------------------------------------------------------
	void* MemoryPool::Allocate()
	{
		if (chunks.empty())
		{
			ASSERT(false, "All blocks in Memory Pool are allocated!");
			return;
		}
		void* ptr = chunks.top();
		chunks.pop();
		return ptr;
	}
	// ------------------------------------------------------------------------
	void MemoryPool::Deallocate(void* ptr)
	{
		chunks.push(ptr);
	}
} // namespace Portal