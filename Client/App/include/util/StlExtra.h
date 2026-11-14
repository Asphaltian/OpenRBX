#ifndef UTIL_STLEXTRA_H
#define UTIL_STLEXTRA_H

#include "placeholder.h"

#include <algorithm>
#include <vector>

namespace RBX {

template <class T>
void fastRemoveIndex(std::vector<T>& vec, unsigned int index)
{
	unsigned int newSize = vec.size() - 1;

	if (index < newSize) {
		vec[index] = vec.back();
	}

	vec.resize(newSize);
}

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
