#ifndef UTIL_STLEXTRA_H
#define UTIL_STLEXTRA_H

#include "decomp.h"

#include <algorithm>
#include <vector>

namespace RBX {

// clang-format off
// TEMPLATE: WEBSERVICE 0x100d3720
// RBX::fastRemoveIndex<RBX::ContactConnector *>
// clang-format on

template <class T>
DECOMP_NOINLINE void fastRemoveIndex(std::vector<T>& newSize, unsigned int index)
{
	unsigned int size = newSize.size();

	if (index < size - 1) {
		newSize[index] = *(newSize.end() - 1);
	}

	newSize.resize(size - 1);
}

// TEMPLATE: WEBSERVICE 0x10103660
// RBX::fastRemoveShort<RBX::Assembly *>

template <class T>
DECOMP_NOINLINE unsigned int fastRemoveShort(std::vector<T>& vec, const T& item)
{
	typename std::vector<T>::iterator found = std::find(vec.begin(), vec.end(), item);
	unsigned int lastOne = found - vec.begin();
	typename std::vector<T>::iterator last = vec.end();

	--last;

	if (found != last) {
		*found = *last;
	}

	vec.resize(vec.size() - 1);

	return lastOne;
}

} // namespace RBX

#endif // UTIL_STLEXTRA_H
