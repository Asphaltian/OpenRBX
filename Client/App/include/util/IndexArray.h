#ifndef UTIL_INDEXARRAY_H
#define UTIL_INDEXARRAY_H

#include "decomp.h"

#include <G3D/Array.h>
#include <boost/noncopyable.hpp>

namespace RBX {

// SIZE 0x0c
template <class T, int& (T::*getIndex)()>
class IndexArray : public boost::noncopyable
{
private:
	G3D::Array<T*> array; // 0x00

	int& indexOf(T* element) const { return (element->*getIndex)(); }

public:
	void fastAppend(T* element)
	{
		indexOf(element) = array.size();
		array.append(element);
	}

	void fastRemove(T* element)
	{
		int index = indexOf(element);
		T* last = array[array.size() - 1];

		array[index] = last;
		indexOf(last) = index;

		array.resize(array.size() - 1, false);

		indexOf(element) = -1;
	}

	bool fastContains(T* element) const;

	const G3D::Array<T*>& underlyingArray() const { return array; }

	T* operator[](unsigned int index) const { return array[index]; }
	T* operator[](int index) const { return array[index]; }
	T* operator[](unsigned int index) { return array[index]; }
	T* operator[](int index) { return array[index]; }

	int size() const { return array.size(); }
};

} // namespace RBX

#endif // UTIL_INDEXARRAY_H
