#ifndef UTIL_SCOPEDASSIGN_H
#define UTIL_SCOPEDASSIGN_H

#include "placeholder.h"

namespace RBX {

template <class T>
class ScopedAssign
{
public:
	ScopedAssign(T& value, const T& newValue) : value(&value), oldValue(value) { *this->value = newValue; }

	~ScopedAssign() { *value = oldValue; }

private:
	T* value;
	T oldValue;
};

} // namespace RBX

#endif // UTIL_SCOPEDASSIGN_H
