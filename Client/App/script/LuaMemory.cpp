#include "script/LuaMemory.h"

#define NOMINMAX

#include <algorithm>
#include <boost/pool/singleton_pool.hpp>
#include <stdlib.h>
#include <string.h>

// FUNCTION: WEBSERVICE 0x100b84e0
LuaAllocator::LuaAllocator() : heapSize(0), heapCount(0), maxHeapSize(0), maxHeapCount(0)
{
}

// FUNCTION: WEBSERVICE 0x100b84f0
void LuaAllocator::clearHeapMax()
{
	maxHeapSize = 0;
	maxHeapCount = 0;
}

// FUNCTION: WEBSERVICE 0x100b8500
void LuaAllocator::getHeapStats(
	unsigned int& heapSize,
	unsigned int& heapCount,
	unsigned int& maxHeapSize,
	unsigned int& maxHeapCount
) const
{
	heapSize = this->heapSize;
	heapCount = this->heapCount;
	maxHeapSize = this->maxHeapSize;
	maxHeapCount = this->maxHeapCount;
}

// FUNCTION: WEBSERVICE 0x100b87e0
template <size_t smallSize, size_t largeSize>
void* bucketRealloc(void* ptr, size_t osize, size_t nsize)
{
	void* result;

	if (nsize > largeSize) {
		if (osize > largeSize || osize == 0) {
			result = realloc(ptr, nsize);
		}
		else {
			result = malloc(nsize);
			memcpy(result, ptr, osize);
			if (osize > smallSize) {
				boost::singleton_pool<LuaAllocator, largeSize>::free(ptr);
			}
			else {
				boost::singleton_pool<LuaAllocator, smallSize>::free(ptr);
			}
		}
	}
	else if (nsize > smallSize) {
		if (osize > largeSize) {
			result = boost::singleton_pool<LuaAllocator, largeSize>::malloc();
			memcpy(result, ptr, nsize);
			free(ptr);
		}
		else if (osize > smallSize) {
			result = ptr;
		}
		else {
			result = boost::singleton_pool<LuaAllocator, largeSize>::malloc();
			if (osize > 0) {
				memcpy(result, ptr, osize);
				boost::singleton_pool<LuaAllocator, smallSize>::free(ptr);
			}
		}
	}
	else if (nsize > 0) {
		if (osize > largeSize) {
			result = boost::singleton_pool<LuaAllocator, smallSize>::malloc();
			memcpy(result, ptr, nsize);
			free(ptr);
		}
		else if (osize > smallSize) {
			result = boost::singleton_pool<LuaAllocator, smallSize>::malloc();
			memcpy(result, ptr, nsize);
			boost::singleton_pool<LuaAllocator, largeSize>::free(ptr);
		}
		else if (osize > 0) {
			result = ptr;
		}
		else {
			result = boost::singleton_pool<LuaAllocator, smallSize>::malloc();
		}
	}
	else {
		if (osize > largeSize) {
			free(ptr);
		}
		else if (osize > smallSize) {
			boost::singleton_pool<LuaAllocator, largeSize>::free(ptr);
		}
		else if (osize > 0) {
			boost::singleton_pool<LuaAllocator, smallSize>::free(ptr);
		}
		result = NULL;
	}

	return result;
}

// FUNCTION: WEBSERVICE 0x100b8970
void* LuaAllocator::alloc(void* ptr, unsigned int osize, unsigned int nsize)
{
	void* result = bucketRealloc<16, 32>(ptr, osize, nsize);

	heapSize += nsize - osize;

	if (osize == 0) {
		heapCount++;
	}

	if (nsize == 0) {
		heapCount--;
	}

	maxHeapSize = std::max(maxHeapSize, heapSize);
	maxHeapCount = std::max(maxHeapCount, heapCount);

	return result;
}

// FUNCTION: WEBSERVICE 0x100b89d0
void* LuaAllocator::alloc(void* ud, void* ptr, unsigned int osize, unsigned int nsize)
{
	return static_cast<LuaAllocator*>(ud)->alloc(ptr, osize, nsize);
}
