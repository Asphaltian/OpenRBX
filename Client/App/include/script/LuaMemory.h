#ifndef SCRIPT_LUAMEMORY_H
#define SCRIPT_LUAMEMORY_H

#include "placeholder.h"

#include <stddef.h>

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
	unsigned int heapSize;
	unsigned int heapCount;
	unsigned int maxHeapSize;
	unsigned int maxHeapCount;
};

#endif // SCRIPT_LUAMEMORY_H
