#ifndef UTIL_IMETRIC_H
#define UTIL_IMETRIC_H

#include "decomp.h"

#include <string>

namespace RBX {

// SIZE 0x4
class IMetric
{
public:
	IMetric() {}
	virtual ~IMetric() {} // vtable+0x00

	virtual std::string getMetric(const std::string&) const = 0; // vtable+0x04
};

DECOMP_SIZE_ASSERT(IMetric, 0x4)

} // namespace RBX

#endif // UTIL_IMETRIC_H
