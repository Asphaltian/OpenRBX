#ifndef UTIL_HITTESTFILTER_H
#define UTIL_HITTESTFILTER_H

#include "placeholder.h"

namespace RBX {

class Primitive;

class HitTestFilter
{
public:
	enum Result
	{
		STOP_TEST = 0,
		IGNORE_PRIM = 1,
		INCLUDE_PRIM = 2
	};

	virtual Result filterResult(const Primitive* primitive) const = 0;
};

} // namespace RBX

#endif // UTIL_HITTESTFILTER_H
