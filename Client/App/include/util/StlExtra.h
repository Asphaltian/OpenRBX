#ifndef UTIL_STLEXTRA_H
#define UTIL_STLEXTRA_H

#include "decomp.h"

#include <algorithm>
#include <vector>

namespace RBX {

// clang-format off
// TEMPLATE: WEBSERVICE 0x100d3720
// STUB: ROBLOX 0x0060aee0
// RBX::fastRemoveIndex<RBX::ContactConnector *>
// TEMPLATE: WEBSERVICE 0x100d40c0
// TEMPLATE: ROBLOX 0x0060b0a0
// RBX::fastRemoveIndex<bool>
// clang-format on

template <class T>
void fastRemoveIndex(std::vector<T>& vec, unsigned int index)
{
	unsigned int newSize = vec.size() - 1;

	if (index < newSize) {
		vec[index] = vec.back();
	}

	vec.resize(newSize);
}

// TEMPLATE: WEBSERVICE 0x10103660
// STUB: ROBLOX 0x005f4fb0
// RBX::fastRemoveShort<RBX::Assembly *>

template <class T>
unsigned int fastRemoveShort(std::vector<T>& vec, const T& item)
{
	typename std::vector<T>::iterator found = std::find(vec.begin(), vec.end(), item);
	unsigned int index = found - vec.begin();
	typename std::vector<T>::iterator lastOne = vec.end();

	--lastOne;

	if (found != lastOne) {
		*found = *lastOne;
	}

	vec.resize(vec.size() - 1);

	return index;
}

} // namespace RBX

#endif // UTIL_STLEXTRA_H
