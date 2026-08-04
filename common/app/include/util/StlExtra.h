#ifndef UTIL_STLEXTRA_H
#define UTIL_STLEXTRA_H

#include "decomp.h"

#include <algorithm>
#include <vector>

namespace RBX {

// TEMPLATE: WEBSERVICE 0x10103660
// RBX::fastRemoveShort<RBX::Assembly *>

template <class T>
DECOMP_NOINLINE unsigned int fastRemoveShort(std::vector<T>& items, const T& item)
{
	typename std::vector<T>::iterator found = std::find(items.begin(), items.end(), item);
	unsigned int index = found - items.begin();
	typename std::vector<T>::iterator last = items.end();

	--last;

	if (found != last) {
		*found = *last;
	}

	items.resize(items.size() - 1);

	return index;
}

} // namespace RBX

#endif // UTIL_STLEXTRA_H
