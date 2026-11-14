#ifndef UTIL_ICONTROLLABLE_H
#define UTIL_ICONTROLLABLE_H

#include "placeholder.h"

namespace RBX {

class __declspec(novtable) IControllable
{
public:
	IControllable() {}

	virtual ~IControllable() {}
	virtual bool isControllable() const = 0;
};

} // namespace RBX

#endif // UTIL_ICONTROLLABLE_H
