#ifndef SCRIPT_LUAMEMORY_H
#define SCRIPT_LUAMEMORY_H

#include "decomp.h"

#include <stddef.h>

// SIZE 0x10
class LuaAllocator
{
public:
	LuaAllocator();

	void clearHeapMax();
	void getHeapStats(unsigned int& heapSize, unsigned int& heapCount) const;
	void getHeapStats(
		unsigned int& heapSize, unsigned int& heapCount, unsigned int& maxHeapSize, unsigned int& maxHeapCount
	) const;

	static void* alloc(void* ud, void* ptr, size_t osize, size_t nsize);
	void* alloc(void* ptr, size_t osize, size_t nsize);

private:
	unsigned int heapSize;     // 0x00
	unsigned int heapCount;    // 0x04
	unsigned int maxHeapSize;  // 0x08
	unsigned int maxHeapCount; // 0x0c
};

DECOMP_SIZE_ASSERT(LuaAllocator, 0x10)

#endif // SCRIPT_LUAMEMORY_H
