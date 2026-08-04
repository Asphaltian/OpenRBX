#ifndef UTIL_INDEXARRAY_H
#define UTIL_INDEXARRAY_H

#include "decomp.h"

#include <G3D/Array.h>

namespace RBX {

// SIZE 0x0c
template <class T, int& (T::*getIndex)()>
class IndexArray
{
public:
	const G3D::Array<T*>& getArray() const { return array; }

	int size() const { return array.size(); }

	T* operator[](int index) const { return array[index]; }

	void fastAppend(T* element)
	{
		(element->*getIndex)() = array.size();
		array.append(element);
	}

	void fastRemove(T* element)
	{
		int index = (element->*getIndex)();
		T* last = array[array.size() - 1];

		array[index] = last;
		(last->*getIndex)() = index;

		array.resize(array.size() - 1, false);

		(element->*getIndex)() = -1;
	}

private:
	G3D::Array<T*> array; // 0x00
};

} // namespace RBX

#endif // UTIL_INDEXARRAY_H
