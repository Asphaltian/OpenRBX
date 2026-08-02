#ifndef UTIL_INDEXARRAY_H
#define UTIL_INDEXARRAY_H

#include "decomp.h"

#include <G3D/Array.h>

namespace RBX {

// SIZE 0x0c
template <class T, int& (T::*getIndex)() const>
class IndexArray
{
public:
	int size() const { return array.size(); }

	T* operator[](int index) const { return array[index]; }

private:
	G3D::Array<T*> array; // 0x00
};

} // namespace RBX

#endif // UTIL_INDEXARRAY_H
