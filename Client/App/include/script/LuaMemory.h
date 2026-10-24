#ifndef SCRIPT_LUAMEMORY_H
#define SCRIPT_LUAMEMORY_H

#include "decomp.h"

// SIZE 0x10
class LuaAllocator
{
public:
	LuaAllocator();

	void clearHeapMax();
	void getHeapStats(unsigned int& heapSize, unsigned int& heapCount) const;
	void getHeapStats(
		unsigned int& heapSize,
		unsigned int& heapCount,
		unsigned int& maxHeapSize,
		unsigned int& maxHeapCount
	) const;

	static void* alloc(void* ud, void* ptr, unsigned int osize, unsigned int nsize);
	void* alloc(void* ptr, unsigned int osize, unsigned int nsize);

private:
	unsigned int heapSize;     // 0x00
	unsigned int heapCount;    // 0x04
	unsigned int maxHeapSize;  // 0x08
	unsigned int maxHeapCount; // 0x0c
};

DECOMP_SIZE_ASSERT(LuaAllocator, 0x10)

#endif // SCRIPT_LUAMEMORY_H
