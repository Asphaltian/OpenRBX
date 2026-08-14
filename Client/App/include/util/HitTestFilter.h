#ifndef UTIL_HITTESTFILTER_H
#define UTIL_HITTESTFILTER_H

#include "decomp.h"

namespace RBX {

class Primitive;

// VTABLE: WEBSERVICE 0x10239d50
// SIZE 0x04
class HitTestFilter
{
public:
	enum Result
	{
		STOP_TEST = 0,
		IGNORE_PRIM = 1,
		INCLUDE_PRIM = 2
	};

	virtual Result filterResult(const Primitive* primitive) const = 0; // vtable+0x00
};

} // namespace RBX

#endif // UTIL_HITTESTFILTER_H
